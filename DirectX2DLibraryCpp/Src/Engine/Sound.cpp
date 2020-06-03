#include <stdio.h>
#include <map>
#include <vector>
#include "Window.h"
#include "Sound.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
// mmioで必要
#pragma comment(lib, "winmm.lib")

bool Sound::Initialize()
{
	// DirectSoundの生成
	if (FAILED(DirectSoundCreate8(
			nullptr,			// GUID
			&m_Interface,	// 生成したDirectSound保存先
			nullptr)))			// NULL固定
	{
			return false;
	}

	// 協調レベルの設定
	if (FAILED(m_Interface->SetCooperativeLevel(
			FindWindow(WINDOW_CLASS_NAME, nullptr),		// ウィンドウハンドル
			DSSCL_NORMAL)))								// 協調レベル
	{
			return false;
	}

	return true;
}

void Sound::Release()
{
	ReleaseAllSoundFiles();

	// 全解放
	for (auto& buffer : m_DuplicateList)
	{
		if (buffer == nullptr)
		{
			continue;
		}

		buffer->Stop();
		buffer->Release();
		buffer = nullptr;
	}
	m_DuplicateList.clear();

	// DirectSoundインターフェースの解放
	if (m_Interface != nullptr)
	{
		m_Interface->Release();
		m_Interface = nullptr;
	}
}

bool Sound::LoadSoundFile(const char* keyword, const char* file_name)
{
	WavData wav_data;
	
	if (LoadWavFile(file_name, &wav_data) == false)
	{
		return false;
	}

	// バッファ情報の設定
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = wav_data.Size;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = &wav_data.WavFormat;

	// セカンダリバッファ作成
	if (FAILED(m_Interface->CreateSoundBuffer(
			&dsbd,							// バッファ情報
			&m_BufferList[keyword],	// 作成されたバッファの保存先
			nullptr)))
	{
		// 作成失敗
		delete[] wav_data.SoundBuffer;
		return false;
	}

	// 波形データを書き込むためにセカンダリバッファをロックする
	void* buffer;
	DWORD buffer_size;
	if (FAILED(m_BufferList[keyword]->Lock(
		0,				// オフセット
		wav_data.Size,	// ロックするバッファサイズ
		&buffer,		// ロックされたバッファパート１の保存先
		&buffer_size,	// ロックされたバッファパート１のサイズ
		nullptr,		// ロックされたバッファパート２の保存先
		nullptr,		// ロックされたバッファパート２のサイズ
		0)))			// ロックオプション
	{
		// ロック失敗
		delete[] wav_data.SoundBuffer;
		return false;
	}

	memcpy(buffer, wav_data.SoundBuffer, buffer_size);

	m_BufferList[keyword]->Unlock(
		&buffer,		// アンロックするバッファパート１
		buffer_size,	// パート１のバッファサイズ
		nullptr,		// アンロックするバッファパート２
		0);				// パート２のバッファサイズ

	// コピーが終わったのでサウンドデータを解放
	delete[] wav_data.SoundBuffer;

	return true;
}

void Sound::ReleaseSoundFile(const char* keyword)
{
	if (m_BufferList.count(keyword) > 0)
	{
		m_BufferList[keyword]->Stop();
		m_BufferList[keyword]->Release();
		m_BufferList[keyword] = nullptr;
		m_BufferList.erase(keyword);
	}
}

void Sound::ReleaseAllSoundFiles()
{
	// セカンダリバッファの解放
	for (auto& buffer : m_BufferList)
	{
		if (buffer.second != nullptr)
		{
			buffer.second->Stop();
			buffer.second->Release();
			buffer.second = nullptr;
		}
	}

	m_BufferList.clear();
}

void Sound::Play(const char* keyword, bool is_loop)
{
	if (m_BufferList.count(keyword) == 0)
	{
		return;
	}

	const DWORD Default = 0;
	DWORD loop_bit = (is_loop ? DSBPLAY_LOOPING : Default);
	// 再生
	m_BufferList[keyword]->Play(
		0,
		0,
		DSBPLAY_LOOPING & loop_bit);
}

void Sound::PlayDuplicate(const char* keyword)
{
	if (m_BufferList.count(keyword) == 0)
	{
		return;
	}

	// バッファの複製と追加
	m_DuplicateList.emplace_back();
	int last_id = (int)m_DuplicateList.size() - 1;
	m_Interface->DuplicateSoundBuffer(
		m_BufferList[keyword],	// 複製元
		&m_DuplicateList[last_id]);		// 複製先

	// 複製したバッファで再生
	m_DuplicateList[last_id]->Play(0, 0, 0);
}

void Sound::Stop(const char* keyword)
{
	if (m_BufferList[keyword] == nullptr)
	{
		return;
	}

	// 停止
	m_BufferList[keyword]->Stop();
	// 再生位置を先頭に戻す
	m_BufferList[keyword]->SetCurrentPosition(0);
}

void Sound::EraseDuplicateSound()
{
	for (auto itr = m_DuplicateList.begin(); itr != m_DuplicateList.end(); )
	{
		bool is_erase = false;

		if (*itr == nullptr)
		{
			is_erase = true;
		}
		else
		{
			DWORD status;

			// 状態取得
			(*itr)->GetStatus(&status);

			if (!(status & DSBSTATUS_PLAYING))
			{
				(*itr)->Release();
				(*itr) = nullptr;

				is_erase = true;
			}
		}

		if (is_erase == true)
		{
			itr = m_DuplicateList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

bool Sound::LoadWavFile(const char* file_name, WavData* out_wave_data)
{
	// WindowsマルチメディアAPIのハンドル
	HMMIO mmio_handle = nullptr;

	// チャンク情報
	MMCKINFO ck_info;
	// RIFFチャンク用
	MMCKINFO riffck_info;

	// WAVファイル内音サンプルのサイズ
	DWORD dwWavSize = 0;

	// WAVファイルを開く
	mmio_handle = mmioOpen(
		(char*)file_name,	// ファイル名
		nullptr,			// MMIO情報
		MMIO_READ);			// オープンモード

	if (mmio_handle == nullptr)
	{
		// オープン失敗
		return false;
	}

	// RIFFチャンクに進入するためにfccTypeにWAVEを設定する
	riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFFチャンクに侵入する
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,	// MMIOハンドル
		&riffck_info,	// 取得したチャンクの情報
		nullptr,		// 親チャンク
		MMIO_FINDRIFF))	// 取得情報の種類
	{
		// 失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// 進入先のチャンクを"fmt "として設定する
	ck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK))
	{
		// fmtチャンクがない
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmtデータの読み込み
	LONG read_size = mmioRead(
		mmio_handle,						// ハンドル
		(HPSTR)&out_wave_data->WavFormat,	// 読み込み用バッファ
		sizeof(out_wave_data->WavFormat));	// バッファサイズ

	if (read_size != sizeof(out_wave_data->WavFormat))
	{
		// 読み込みサイズが一致してないのでエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットチェック
	if (out_wave_data->WavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		// フォーマットエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmtチャンクを退出する
	if (mmioAscend(mmio_handle, &ck_info, 0) != MMSYSERR_NOERROR)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// dataチャンクに進入する
	ck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		// 進入失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// サイズを保存
	out_wave_data->Size = ck_info.cksize;

	// dataチャンク読み込み
	out_wave_data->SoundBuffer = new char[ck_info.cksize];
	read_size = mmioRead(mmio_handle, (HPSTR)out_wave_data->SoundBuffer, ck_info.cksize);
	if (read_size != ck_info.cksize)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		delete[] out_wave_data->SoundBuffer;
		return false;
	}

	// ファイルを閉じる
	mmioClose(mmio_handle, MMIO_FHOPEN);

	return true;
}
