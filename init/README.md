## ![icon](../readme_pics/softdown-ico-pc9801-small.png) INIT：グラフィックス、テキスト、ビープ音初期化 for PC9801<!-- omit in toc -->

---
[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [setkeytable](../init/README.md) > ***init*** (this page)

<br />
<br />

Last Updated : Jan. 1993


- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
  - [初期化内容](#初期化内容)
- [動作環境](#動作環境)
- [バージョンアップ情報](#バージョンアップ情報)
- [ライセンス](#ライセンス)

<br />
<br />

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../init/download) 

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [GoogleDriveを参照する](https://drive.google.com/drive/folders/0B7BSijZJ2TAHY2UzMWI5NDQtZWRjYi00MTdlLThlMGUtYmUwMDg2NGJkYmIy?resourcekey=0-0g55CdK32ZnItl6j99GNZA) 

<br />
<br />

## 機能の概要

グラフィックス描画モード、テキスト描画モード、ビープ音周波数を初期化します

作法の悪いソフトウエア実行後に、「ビープ音が変になった」「カーソルが消えた」「グラフィックスの絵が画面上に残っている」などの変な状態を元に戻します 

- INIT8  : PC-9801 起動時モード、8 色モード
- INIT16 : INIT8 を 4096 色中 16 色モードにしたもの


### 初期化内容

- グラフィックス画面（裏画面はいじりません）の消去と表示停止
- グラフィックスパレットの初期化
- グラフィックスモードを４００ラインにする
- アクティブ画面を表画面にする
- ボーダーカラーを消去
- テキスト画面の消去と表示開始
- ファンクションキーの表示と表示色を白とする
- ﾊﾞｰﾃｨｶﾙﾗｲﾝ ﾓｰﾄﾞ にする
- ﾌｫﾝﾄ を PC-9801 のものにする
- ビープ音の周波数を２０００Ｈｚにする

<br />
<br />

## 動作環境

- PC-9801 MS-DOS Ver 3.3x 以降

<br />
<br />

## バージョンアップ情報

-  Version 1.01 (1993/01/14) 

<br />
<br />

## ライセンス

このソフトウエアは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア
