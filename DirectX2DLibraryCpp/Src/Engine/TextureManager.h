/**
* @file TextureManager.h
* @brief <pre>
* テクスチャの読み込み、解放などの管理を行うクラスの宣言
* Engineクラスでインスタンスを作成するので使用者が作成する必要はない
* TextureManagerクラスの関数はEngineクラス、またはEngine内で使用するため、利用者が直接使用しない
* </pre>
*/
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <map>
#include "EngineConstant.h"

/** @brief テクスチャファイルの管理クラス */
class TextureManager
{
public:
	/**
	* @brief 初期化関数
	* @details ゲームで使用するテクスチャデータを保存出来るようにする
	*/
	void Initialize();

	/**
	* @brief 解放関数
	* @details このクラスで管理しているデータを解放する
	*/
	void Release();

	/**
	* @brief テクスチャ読み込み関数
	* @details 指定したされたテクスチャファイルを読み込み、keywordの文字列で登録する
	* @retval true 読み込み成功
	* @retval false 読み込み失敗
	* @param[in] keyword 登録用キーワード
	* @param[in] file_name 読み込むテクスチャ名(パス込み)
	*/
	bool LoadTexture(const char* keyword, const char* file_name);

	/**
	* @brief テクスチャ全解放関数
	* @details 読み込んでいるすべてのテクスチャを解放する
	*/
	void ReleaseAllTextures();

	/**
	* @brief テクスチャ解放関数
	* @details 指定されたーキーワードのテクスチャを解放する
	* @param[in] keyword 解放するテクスチャのキーワード
	*/
	void ReleaseTexture(const char* keyword);

	/**
	* @brief テクスチャデータの取得関数
	* @details 指定されたキーワードのテクスチャデータを取得する
	* @retval Texture* テクスチャデータ(取得失敗時はnullptr)
	* @param[in] keyword 取得したいテクスチャのキーワード
	*/
	Texture* GetTexture(const char* keyword);

private:
	std::map<const char*, Texture> m_TextureList; //!< テクスチャリスト
};

#endif
