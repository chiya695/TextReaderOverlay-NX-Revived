# TextReaderOverlay-NX Revived锛堜腑鏂囪鏄庯級

杩欐槸 [diwo/TextReaderOverlay-NX](https://github.com/diwo/TextReaderOverlay-NX) 鐨勫吋瀹规€х淮鎶ゅ垎鏀€傚畠鏄竴涓繍琛屽湪 Nintendo Switch Atmosphere/Tesla Menu 涓婄殑鏂囨湰闃呰 Overlay锛屽彲浠ュ湪娓告垙鎴栧叾浠栧墠鍙扮▼搴忎笂鏂规祻瑙?SD 鍗′腑鐨?`.txt` 鏂囦欢銆?
## 涓轰粈涔堣淇敼

鍘熼」鐩殑鍔熻兘璁捐寰堝疄鐢紝浣嗗師濮嬩唬鐮佷緷璧栬緝鏃х殑 libnx銆乴ibtesla 鍜?HID 鎺ュ彛銆傞殢鐫€ Atmosphere 鍜屽紑鍙戝伐鍏烽摼鏇存柊锛屾棫鐗堜細鍑虹幇鎻掍欢涓嶆敮鎸併€丅rowser 宕╂簝銆佹棤娉曟墦寮€ `sdmc:`銆佹墦寮€鏂囨湰鍚庝竴鐗囩┖鐧界瓑闂銆?
鏈垎鏀笉鏄彧鍋氣€滆兘缂栬瘧鈥濈殑淇敼锛岃€屾槸鍦ㄧ湡瀹?Switch 涓婄粨鍚?crash report 鍜岄€愮増瀹炴満娴嬭瘯瀹屾垚锛?
- 鏇存柊鏃х増 HID API 鍜?libtesla 鎺ュ彛锛?- 淇鏃х増瀛椾綋搴撳鑷寸殑鍐呭瓨涓嶈冻锛?- 鍚姩鏃舵樉寮忔寕杞?SD 鍗★紝閫€鍑烘椂鍗歌浇锛?- 淇鐩綍娴忚鍜岃矾寰勫鐞嗭紱
- 淇鏂扮増 libtesla 涓嬫牴绾?`CustomDrawer` 灏哄涓?0 瀵艰嚧鐨勬枃瀛楀叏閮ㄨ瑁佸壀锛?- 澧炲姞 UTF-8銆乁TF-16銆乁TF-32銆丟B18030/GBK銆乄indows-1252 鑷姩妫€娴嬶紱
- 浣跨敤 Switch 绯荤粺鍏变韩瀛椾綋鏄剧ず涓棩闊╂枃瀛楋紝涓嶅啀鎼哄甫澶у瀷 CJK 瀛椾綋锛?- 淇鐭枃鏈湯灏惧嚭鐜版棤鍏冲爢鍐呭瓨鍐呭鐨勯棶棰橈紱
- 淇 Help 椤甸潰鍦ㄦ柊鐗?libtesla 涓嬬殑瑁佸壀闂銆?
## 浣跨敤鏉′欢

- 宸插畨瑁?Atmosphere 鐨?Nintendo Switch锛?- 鏈€鏂扮増 `nx-ovlloader`锛?- 鏈€鏂扮増 Tesla Menu锛?- SD 鍗′腑瀛樺湪鎵╁睍鍚嶄负 `.txt` 鐨勬枃鏈枃浠躲€?
鐩墠宸插湪 Atmosphere 1.11.2 / 绯荤粺鐗堟湰 22.5.0 鐜涓畬鎴愬疄鏈烘祴璇曪紝鍏朵粬鐗堟湰涓嶄綔缁濆淇濊瘉銆?
## 瀹夎

1. 灏嗘渶鏂扮増 [nx-ovlloader](https://github.com/WerWolv/nx-ovlloader) 瀹夎鍒?SD 鍗★紱
2. 灏嗘渶鏂扮増 [Tesla Menu](https://github.com/WerWolv/Tesla-Menu) 瀹夎鍒?SD 鍗★紱
3. 浠庢湰浠撳簱 Releases 涓嬭浇 ZIP锛屽苟瑙ｅ帇鍒?SD 鍗℃牴鐩綍锛?4. 灏?`.txt` 鏂囦欢鏀惧埌 SD 鍗′换鎰忎綅缃紱
5. 瀹夎鎴栨浛鎹?Overlay 鍚庡缓璁畬鏁撮噸鍚?Switch锛?6. 浣跨敤 `L + 鍗佸瓧閿笅 + 鍙虫憞鏉嗘寜涓媊 鎵撳紑 Tesla Menu锛?7. 閫夋嫨 `Text Reader`锛屽啀杩涘叆 `Browse...` 閫夋嫨鏂囨湰銆?
濡傛灉鏇挎崲鍚庝粛鍍忔棫鐗堟湰锛屽垹闄?`sd:/switch/.overlays/TextReaderOverlay-NX.ovl`锛岄噸鏂板鍒舵柊鐗堟湰锛岀劧鍚庨噸鍚?Switch銆備粎閫€鍑?Tesla 鎴栭殣钘?Overlay锛屼笉涓€瀹氫細璁╁凡缁忛┗鐣欏唴瀛樼殑鏃т唬鐮佹秷澶便€?
## 鎸夐敭

### 鏂囦欢娴忚

- `A`锛氭墦寮€鐩綍鎴栨枃鏈紱
- `Y`锛氭坊鍔?鍙栨秷 Favorite锛?- `B`锛氳繑鍥炰笂涓€绾с€?
### 闃呰鍣?
- 宸︽憞鏉嗭細婊氬姩锛?- `ZL + 宸︽憞鏉哷锛氬姞閫熸粴鍔紱
- `ZR + 宸︽憞鏉哷锛氬揩閫熸粴鍔紝鎴栬烦鍒伴《閮?搴曢儴锛?- 鍙虫憞鏉嗕笂涓嬶細缁嗗井婊氬姩锛?- 鍙虫憞鏉嗗乏鍙筹細宸﹀彸骞崇Щ锛?- 鎸変笅鍙虫憞鏉嗭細閲嶇疆姘村钩浣嶇疆锛?- 鍗佸瓧閿笂涓嬶細璋冩暣瀛椾綋澶у皬锛?- `Y`锛氬垏鎹功绛撅紱
- `L/R`锛氫笂涓€涓?涓嬩竴涓功绛撅紱
- `X`锛氶殣钘?Overlay锛?- `B`锛氬叧闂槄璇诲櫒锛?- `-`锛氭樉绀?闅愯棌璇婃柇淇℃伅銆?
## 鏂囨湰缂栫爜鏀寔

绋嬪簭鎸夋祦寮忓垎鍧楁柟寮忚鍙栵紝涓嶄細鎶婃暣鏈ぇ鏂囦欢涓€娆℃€у姞杞藉埌鍐呭瓨锛屾敮鎸侊細

- UTF-8锛堟湁鎴栨棤 BOM锛夛紱
- UTF-16 LE/BE锛?- UTF-32 LE/BE锛?- GB18030/GBK锛?- Windows-1252銆?
鏂囧瓧浣跨敤 Switch 绯荤粺鍏变韩瀛椾綋娓叉煋锛屾敮鎸佹媺涓佹枃銆佹棩鏂囥€佺畝浣撲腑鏂囥€佺箒浣撲腑鏂囧拰闊╂枃锛屽苟閬垮厤鍦?Overlay 涓惡甯﹀ぇ鍨嬩腑鏂囧瓧搴撱€?
## 鍘熷鍑哄涓庤鍙瘉

- 鍘熷椤圭洰锛歔diwo/TextReaderOverlay-NX](https://github.com/diwo/TextReaderOverlay-NX)锛?- Overlay 妗嗘灦锛歔WerWolv/libtesla](https://github.com/WerWolv/libtesla)锛?- Overlay 鍔犺浇鍣細[WerWolv/nx-ovlloader](https://github.com/WerWolv/nx-ovlloader)锛?- Tesla Menu锛歔WerWolv/Tesla-Menu](https://github.com/WerWolv/Tesla-Menu)锛?- Atmosphere锛歔Atmosphere-NX/Atmosphere](https://github.com/Atmosphere-NX/Atmosphere)銆?
鏈粨搴撲繚鐣欏師椤圭洰 GPL 璁稿彲璇侊紝璇峰悓鏃堕伒瀹?vendored 渚濊禆鍚勮嚜鐨勮鍙瘉瑕佹眰锛岃瑙?[LICENSE](LICENSE)銆?
杩欐槸闈炲畼鏂圭ぞ鍖虹淮鎶ゅ垎鏀紝涓?Nintendo銆丄tmosphere銆乀esla Menu 鍙婂師浣滆€呮病鏈夐毝灞炲叧绯汇€?
