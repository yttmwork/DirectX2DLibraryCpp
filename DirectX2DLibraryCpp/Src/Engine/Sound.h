/**
* @file Sound.h
* @brief <pre>
* サウンドクラスの宣言
* Engineクラスでインスタンスを作成するので使用者が作成する必要はない
* Soundクラスの関数はEngineクラス、またはEngine内で使用するため、利用者が直接使用しない
* </pre>
*/
#ifndef SOUND_H_
#define SOUND_H_

#include <dsound.h>
#include <map>
#include <vector>

/** @brief Wavデータ格納用 */
struct WavData
{
	WAVEFORMATEX WavFormat; //!< Wavフォーマットデータ
	char* SoundBuffer;		//!< サウンドデータ
	DWORD Size;				//!< サイズ
};

/** @brief サウンドクラス */
class Sound
{
public:
	/**
	* @brief サウンド機能初期化関数
	* @details サウンドを使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize();

	/**
	* @brief サウンド機能終了関数
	* @details サウンド機能を終了させる
	*/
	void Release();

	/**
	* @brief サウンド再生関数
	* @details 指定されたキーワードのサウンドファイルを再生する
	* @param[in] keyword 再生するサウンドのキーワード
	* @param[in] is_loop ループ設定(デフォルトは非ループ)
	*/
	void Play(const char* keyword, bool is_loop = false);

	/**
	* @brief 複製再生関数
	* @details <pre>
	* キーワード指定されたサウンドファイルを複製再生する
	* SEのように同じサウンドを重複して再生する場合に効果的
	* </pre>
	* @param[in] keyword 再生するサウンドのキーワード
	*/
	void PlayDuplicate(const char* keyword);

	/**
	* @brief サウンド停止関数
	* @details <pre>
	* キーワード指定されたサウンドを停止する
	* ※PlayDuplicateを使用して再生しているサウンドは対象外
	* </pre>
	* @param[in] keyword 停止するサウンドのキーワード
	*/
	void Stop(const char* keyword);

	/**
	* @brief サウンドファイルの読み込み関数
	* @details 指定されたファイル名のサウンドファイルを読み込みkeywordの文字列で登録する
	* @retval true 読み込み成功
	* @retval false 読み込み失敗
	* @param[in] keyword 登録用キーワード
	* @param[in] file_name ファイル名
	*/
	bool LoadSoundFile(const char* keyword, const char* file_name);

	/**
	* @brief サウンドファイル解放関数
	* @details 指定されたキーワードのサウンドファイルを解放する
	* @param[in] keyword 解放するサウンドファイルのキーワード
	*/
	void ReleaseSoundFile(const char* keyword);

	/**
	* @brief サウンドファイル全解放関数
	* @details サウンドファイルを全て解放する
	*/
	void ReleaseAllSoundFiles();

	/**
	* @brief 複製再生サウンド削除関数
	* @details <pre>
	* 再生が終了したPlayDuplicateのサウンドに対して終了処理を行う
	* 毎フレーム実行する必要がある
	* </pre>
	*/
	void EraseDuplicateSound();

	/**
	* @brief Wavファイルの読み込み関数
	* @details <pre>
	* 引数で指定したWavファイルを読み込む
	* ※LoadSoundFileで実行する関数なので使用者は実行する必要がない
	* </pre>
	* @retval true 読み込み成功
	* @retval false 読み込み失敗
	* @param[in] file_name ファイル名
	* @param[out] out_wave_data 読み込んだWavデータ
	*/
	bool LoadWavFile(const char* file_name, WavData* out_wave_data);

private:
	LPDIRECTSOUND8 m_Interface = nullptr;						//!< サウンドデバイス
	std::map<const char*, LPDIRECTSOUNDBUFFER> m_BufferList;	//!< サウンドデータ保存用
	std::vector<LPDIRECTSOUNDBUFFER> m_DuplicateList;			//!< 複製バッファ保存用
};

#endif
