## CHKVEC：割り込みベクター監視ソフト for PC9801<!-- omit in toc -->

---
[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [setkeytable](../chkvec/README.md) > ***chkvec*** (this page)

<br />
<br />

Last Updated : Jul. 1992


- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
  - [「割り込み」とは](#割り込みとは)
  - [このソフトウエアの仕組み](#このソフトウエアの仕組み)
  - [ソフトウエアの起動オプション](#ソフトウエアの起動オプション)
  - [標準的な使い方](#標準的な使い方)
- [（参考資料）主な割り込みﾍﾞｸﾀｰ表](#参考資料主な割り込みﾍﾞｸﾀｰ表)
  - [80x86 CPU 又は MS-DOS により予約された割り込み](#80x86-cpu-又は-ms-dos-により予約された割り込み)
  - [ﾕｰｻﾞｰに解放された割り込み](#ﾕｰｻﾞｰに解放された割り込み)
- [動作環境](#動作環境)
- [バージョンアップ情報](#バージョンアップ情報)
- [ライセンス](#ライセンス)

<br />
<br />

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../chkvec/download) 

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [GoogleDriveを参照する](https://drive.google.com/drive/folders/0B7BSijZJ2TAHY2UzMWI5NDQtZWRjYi00MTdlLThlMGUtYmUwMDg2NGJkYmIy?resourcekey=0-0g55CdK32ZnItl6j99GNZA) 

<br />
<br />

## 機能の概要

正常時の割り込みデータ(0 - 255)を全てデータファイルに保存。

以後は、このデータと現在の割り込みベクターを比較して異常を検知。 

### 「割り込み」とは

80x86 CPU には、（アセンブラで）INT xx と言う命令で、２５６種類有るコンピューターの基本機能（？）を使用するジャンプ命令があります。INT xx を「割り込み」と言い、ふつうの関数を呼び出す JMP, Jxx, CALL などとは違いどのプログラムからいつでも（？）呼び出せる特殊な関数を呼び出す事です。

主に、BIOS, MS-DOS, ﾃﾞﾊﾞｲｽﾄﾞﾗｲﾊﾞｰなどがこの関数を持っており、ｷｰﾎﾞｰﾄﾞ割り込み(INT 09h)、BIOS割り込み(INT 18h)、DOS割り込み(INT 21h)…等を聞かれた事があるかも知れません。

CPU がこれらの命令のある場所を見つけるのに「割り込みﾍﾞｸﾀｰ (Interrupt Vector)」を使用します。 割り込みﾍﾞｸﾀｰはメモリのいちばん先頭に置かれ、普通のプログラムではそこをいじる事は滅多にしません（終了時に元に戻すから。TSR などはここを書き換えて常駐する事が多い）。

### このソフトウエアの仕組み

このプログラムはこの割り込みﾍﾞｸﾀｰを保存しておき、以後この値と現在の値を比較する事に依って、割り込みが書き換えられた事を探知しようとする物です。

どうしてこのような事をするのかと言いますと、常駐型のｳｲﾙｽが、割り込みﾍﾞｸﾀｰを書き換えて、ｼｽﾃﾑに侵入した場合に使用者に知らせようと言う物です。

ｳｲﾙｽが何かをしようとする場合、そのｳｲﾙｽが起動するために割り込みﾍﾞｸﾀｰをｳｲﾙｽを指すようにしておけば、その割り込みが（不運にも）呼ばれた場合にｳｲﾙｽが活動できるように出来るわけです。

例えば、ﾃﾞｨｽｸ ｱｸｾｽ の割り込みに感染してﾌｧｲﾙ等がｺﾋﾟｰされる度にｳｲﾙｽ自身を転送させるには、ﾃﾞｨｽｸ関連の割り込みを書き換え、ｳｲﾙｽの感染の仕事を行う関数を経由して元の（正常な）ﾃﾞｨｽｸ割り込みを呼び出す様な事をしてもよいわけです（MS-DOS 内部を書き換えてしまう場合などはﾍﾞｸﾀｰを調べるだけでは対処できない。又、常駐しないｳｲﾙｽには全く効果がない事もある。）。

### ソフトウエアの起動オプション

```
CHKVEC -v -k [datafilename]
```

- -v : ﾃﾞｰﾀﾌｧｲﾙの内容を表示するだけ。
- -k : 割り込みが書き換えられた場合に「何かキーを押して下さい」メッセージを出さない。(PC-9801以外の機種では必ず指定)
- ﾃﾞｰﾀﾌｧｲﾙ名 : CHKVEC.DAT 以外のデータファイル名を使いたいときに指定。

2回以上実行したい方はﾃﾞｰﾀﾌｧｲﾙ名を指定して下さい。

### 標準的な使い方

ﾉｰﾏﾙな使い方：AUTOEXEC.BAT に引き数無しで入れておく。

それまでに実行されたﾌﾟﾛｸﾞﾗﾑについての結果が反映される。

　例

```
組み込み前               組み込み後
----------               ----------
PROMPT $p$g              PROMPT $p$g
SET TMP=D:\              SET TMP=D:\
SET PATH=A:\USR;A:\BIN   SET PATH=A:\USR;A:\BIN
A:\USR\ABF /B20 /LH      A:\USR\ABF /B20 /LH
A:\USR\FCOLOR W          A:\USR\CHKVEC     << 1行挿入
A:\SYS\IOSYSP            A:\USR\FCOLOR W
A:\SELEX\SELEX_EX        A:\SYS\IOSYSP
A:\SELEX\SELEX_MB        A:\SELEX\SELEX_EX
                         A:\SELEX\SELEX_MB
```

挿入位置は何処でも良いです。後の方がよりたくさんの結果を反映します。

<br />
<br />

## （参考資料）主な割り込みﾍﾞｸﾀｰ表

### 80x86 CPU 又は MS-DOS により予約された割り込み

```
00	除算例外処理 (Divide by 0)
01	ｼﾝｸﾞﾙｽﾃｯﾌﾟ割込み
02	ﾒﾓﾘ ｴﾗｰ処理 (NMI = Non Maskable Interrupt)
03	ﾌﾞﾚｰｸﾎﾟｲﾝﾄ割込み
04	ｵｰﾊﾞｰﾌﾛｰ処理
05	COPY ｷｰ 割込み
06	STOP ｷｰ 割込み
07	ｲﾝﾀｰﾊﾞﾙﾀｲﾏ
08	ﾀｲﾏ ﾊｰﾄﾞｳｪｱ割込み
09	ｷｰﾎﾞｰﾄﾞ ﾊｰﾄﾞｳｪｱ割込み
0A	CRT垂直同期信号割込み (V-SYNC)
0B	                        (拡張ﾊﾞｽ INT 0 )
0C	RS232C [ch0]
0D	CMT(ｶｾｯﾄｲﾝﾀｰﾌｪｰｽ)       (拡張ﾊﾞｽ INT 1)
0E	                        (拡張ﾊﾞｽ INT 2 )
0F	ｽﾚｰﾌﾞ割込み
10	ﾌﾟﾘﾝﾀ ﾊｰﾄﾞｳｪｱ割込み
11	固定ﾃﾞｨｽｸ･ﾊｰﾄﾞｳｪｱ割込み (拡張ﾊﾞｽ INT 3 )
12	640KB FDD･ﾊｰﾄﾞｳｪｱ割込み (拡張ﾊﾞｽ INT 41)
13	1MB   FDD･ﾊｰﾄﾞｳｪｱ割込み (拡張ﾊﾞｽ INT 42)
14	RS232C [ch1, ch2]       (拡張ﾊﾞｽ INT 5 )
15	ﾏｳｽ ﾊｰﾄﾞｳｪｱ割込み       (拡張ﾊﾞｽ INT 6 )
16	数値演算ﾌﾟﾛｾｯｻ (80x87 ｺﾌﾟﾛｾｯｻ)
17	ﾉｲｽﾞ (GND接続)
18	ｷｰﾎﾞｰﾄﾞ,画面 BIOS
19	RS232C BIOS
1A	CMT(ｶｾｯﾄｲﾝﾀｰﾌｪｰｽ),ﾌﾟﾘﾝﾀ BIOS
1B	ﾃﾞｨｽｸ BIOS
1C	ｶﾚﾝﾀﾞ時計,ﾀｲﾏ BIOS
1D
1E	ﾌﾞｰﾄｽﾄﾗｯﾌﾟﾛｰﾀﾞ呼出し
1F	80286保護ﾓｰﾄﾞによるﾒﾓﾘｱｸｾｽ
20	ﾌﾟﾛｸﾞﾗﾑの終了
21	MS-DOS ﾌｧﾝｸｼｮﾝ ﾘｸｴｽﾄ
22	終了ｱﾄﾞﾚｽ
23	<CTRL-C>の抜け出しｱﾄﾞﾚｽ
24	致命的ｴﾗｰによる打ち切りｱﾄﾞﾚｽ
25	ｱﾌﾞｿﾘｭｰﾄ ﾃﾞｨｽｸﾘｰﾄﾞ
26	ｱﾌﾞｿﾘｭｰﾄ ﾃﾞｨｽｸﾗｲﾄ
27	ﾌﾟﾛｸﾞﾗﾑの常駐終了
28	ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ処理 (いわゆるDOSのﾏﾙﾁﾀｽｸ､PRINT.EXE,TSR使用)
29	ｺﾝｿｰﾙ出力
2A	MS-Networks ﾈｯﾄﾜｰｸ BIOS
2B
2C
2D
2E	COMMAND.COM 呼び出し
2F	常駐ﾌﾟﾛｾｽとの通信
30	MS-DOS予約 (ﾌｧﾝｸｼｮﾝ ﾘｸｴｽﾄのFAR JMP)
31	MS-DOS予約 (ﾌｧﾝｸｼｮﾝ ﾘｸｴｽﾄのFAR JMP)
32	MS-DOS予約 (ﾌｧﾝｸｼｮﾝ ﾘｸｴｽﾄのFAR JMP)
33	ﾏｳｽ BIOS, ﾏｳｽ ﾄﾞﾗｲﾊﾞ
34
35
36
37
38
39
3A
3B
3C
3D
3E
3F	ｵｰﾊﾞｰﾚｲ ﾛｰﾀﾞ呼び出し
```

### ﾕｰｻﾞｰに解放された割り込み

```
67	LIM EMS BIOS (LIM は Lotus, Intel, Microsoft の規格)
A0	ｸﾞﾗﾌｨｯｸLIO (GINIT)
A1	ｸﾞﾗﾌｨｯｸLIO (GSCREEN)
A2	ｸﾞﾗﾌｨｯｸLIO (GVIEW)
A3	ｸﾞﾗﾌｨｯｸLIO (GCOLOR1)
A4	ｸﾞﾗﾌｨｯｸLIO (GCOLOR2)
A5	ｸﾞﾗﾌｨｯｸLIO (GCLS)
A6	ｸﾞﾗﾌｨｯｸLIO (GPSET)
A7	ｸﾞﾗﾌｨｯｸLIO (GLINE)
A8	ｸﾞﾗﾌｨｯｸLIO (GCLRCLE)
A9	ｸﾞﾗﾌｨｯｸLIO (GPAINT1)
AA	ｸﾞﾗﾌｨｯｸLIO (GPAINT2)
AB	ｸﾞﾗﾌｨｯｸLIO (GGET)
AC	ｸﾞﾗﾌｨｯｸLIO (GPUT1)
AD	ｸﾞﾗﾌｨｯｸLIO (GPUT2)
AE	ｸﾞﾗﾌｨｯｸLIO (GROLL)
AF	ｸﾞﾗﾌｨｯｸLIO (GPOINT2)
CE	ｸﾞﾗﾌｨｯｸLIO (GCOPY) 使わない時もある
DC	PC-9801 拡張機能
```

<br />
<br />


## 動作環境

- PC-9801 MS-DOS

<br />
<br />

## バージョンアップ情報

-  Version 1.0 (1992/07/19) 

<br />
<br />

## ライセンス

このソフトウエアは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア
