## ![icon](../readme_pics/softdown-ico-pc9801-small.png) SETNUM：行番号自動作成 for PC9801<!-- omit in toc -->

---
[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [setkeytable](../setlinenumber/README.md) > ***setlinenumber*** (this page)

<br />
<br />

Last Updated : Apr. 1991


- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
- [動作環境](#動作環境)
- [バージョンアップ情報](#バージョンアップ情報)
- [ライセンス](#ライセンス)

<br />
<br />

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../setlinenumber/download) 

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [GoogleDriveを参照する](https://drive.google.com/drive/folders/0B7BSijZJ2TAHY2UzMWI5NDQtZWRjYi00MTdlLThlMGUtYmUwMDg2NGJkYmIy?resourcekey=0-0g55CdK32ZnItl6j99GNZA) 

<br />
<br />

## 機能の概要

N88-BASICやACOS FORTRANなど行番号が必要なソースコードを作成する場合に、テキストエディターを使って作成した“行番号なし”のプレーンテキストファイルに、行番号を付けるためのソフトウエア 

起動時の引数で機能設定する

```
SETNUM [読み込みファイル] ([書き出しファイル]) (-l開始行) (-sステップ) (-z(ゼロフィックス無し))
```

デフォルト値は、開始行100、ステップ10、行番号は4桁で先頭ゼロフィル有り。 また、書き出しファイル名を指定しない場合は、読み込みファイルに結果を上書きする。

```
SETNUM [読み込みファイル] -l100 -s10
```

行番号はintの限界32767までしか対応しない。

<br />
<br />

## 動作環境

- PC-9801 MS-DOS 3.3x

<br />
<br />

## バージョンアップ情報

-  Version 1.0

    当初

-  Version 2.0 (1991/04/29)

<br />
<br />

## ライセンス

このソフトウエアは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア
