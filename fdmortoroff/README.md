## フロッピーディスク モーター自動停止 for PC9801<!-- omit in toc -->

---
[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [setkeytable](../fdmortoroff/README.md) > ***fdmortoroff*** (this page)

<br />
<br />

Last Updated : Mar. 1992


- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
  - [原理](#原理)
- [動作環境](#動作環境)
  - [ソースコードのビルド方法](#ソースコードのビルド方法)
- [バージョンアップ情報](#バージョンアップ情報)
- [ライセンス](#ライセンス)

<br />
<br />

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../fdmortoroff/download) 

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [GoogleDriveを参照する](https://drive.google.com/drive/folders/0B7BSijZJ2TAHY2UzMWI5NDQtZWRjYi00MTdlLThlMGUtYmUwMDg2NGJkYmIy?resourcekey=0-0g55CdK32ZnItl6j99GNZA) 

<br />
<br />

## 機能の概要

1 MB / 640 KB ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾄﾞﾗｲﾌﾞを 1 MB モードで使用する時に1 MB ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾄﾞﾗｲﾌﾞのモーターを自動的に ON / OFF して消費電力／作動音（ｼｬｶｼｬｶという小さな音）を低減する。

### 原理

PC - 9801 の 1 MB / 640 KB ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾄﾞﾗｲﾌﾞ BIOS でサポートされた新しいファンクションを用いる。従って、メモリー中に本プログラムは常駐しない。

モータ停止モードの設定■
1 MB FD のモータを自動的に ON / OFF するモードに設定する。
このコマンドを発効した後にモータ常時 ON モードに戻す事は出来ない。

- 入力
  - AH := 83H
  - AL := 9xH (x=0,1)DA/UA 装置番号
  - INT 1BH
- 出力
  - CF == 0 : 成功
  - CF == 1 : 失敗（このモードはサポートされない）

FD_MOFF.COM では 1 MB FDD 装置の確認に各装置を INITIALIZE して厳密に行っている。従って動作時間がかかる。

FD_MOFF2.COM では 1 MB FDD 装置の接続状況を PC - 9801 のワークエリアに頼り、動作の高速化が計られている。現在 1 MB モードの装置のみを対象とする。

通常は FD_MOFF2.COM を使えば良いと思います。


<br />
<br />

## 動作環境

- PC-9801 で 1 MB / 640 KB ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾄﾞﾗｲﾌﾞ搭載機種で 1 MB ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾄﾞﾗｲﾌﾞ自動停止モードをサポートしたもの。<br/>
実際は RX(EX),RS(ES),RA,DX,DS,DA 以降の機種等を指す。
- それ以外の 1 MB / 640 KB ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾄﾞﾗｲﾌﾞ搭載機種でも実行は可能だが、1 MB ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾄﾞﾗｲﾌﾞ自動停止モードに出来ないだけである。（害は無いと思われる）
- .COM 形式をサポートする OS (MS-DOS等)

### ソースコードのビルド方法

それぞれのプログラムのソースコード（．ＡＳＭ）はTURBO ASSEMBLER を用いてアセンブル、リンクを行う。

```
TASM FD_MOFF.ASM
TLINK /t FD_MOFF.OBJ
```

<br />
<br />

## バージョンアップ情報

-  Version 1.0 (1992/03/07) 

    当初

<br />
<br />

## ライセンス

このソフトウエアは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア
