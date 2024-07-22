## 使用説明書

1. YaeAchievement（最新バージョン）をダウンロード：

こちらをクリック：https://github.com/HolographicHat/YaeAchievement/releases

赤枠で囲まれた「YaeAchievement.exe」という名前のファイルをクリックすると、自動的にポップアップしてダウンロードされます。このファイルをデスクトップや他の見やすいフォルダに保存することをお勧めします。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide1.png)

2. .NET Runtime 7をインストール（ランタイムが既にインストールされている場合はこのステップを無視できます）

こちらをクリック：https://dotnet.microsoft.com/en-us/download/dotnet/thank-you/runtime-8.0.4-windows-x64-installer .

または、Windows 11を使用しているか、Wingetがインストールされている場合は、`winget install Microsoft.DotNet.Runtime.7`を実行します。

ウェブページにアクセスすると、ブラウザが自動的にポップアップしてダウンロードされます。ファイルをデスクトップや他の見やすいフォルダに保存します。

ダウンロードが完了したら、dotnet-runtime-x.x.x-win-x64.exeという名前のファイルを開くと、インストールウィンドウがポップアップします。以下の図のように表示されます。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide2.png)

インストールをクリックするだけです。

3. メインプログラムを開くための操作と実績エクスポートのオプション

最初のステップでダウンロードした「YaeAchievement.exe」という名前のファイルをダブルクリックして開くと、原神が起動していることを示します。以下の図のように表示されます。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide3.png)

原神の起動が完了したら、ゲームに入ります。

ゲームに入ると、ツールがどのツールにエクスポートするかを選択するように促します。以下の図のように表示されます。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide4.png)

グローバルユーザーの場合、[3] Seelie.meまたは[4] csvファイルにエクスポートを選択する必要があります。

選択後、各ページは次のようにツールをエクスポートします：

#### Snap.Hutao：

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide5.png)

#### Seelie.me：

この時点で、Yae Achievementは実績データがエクスポートされたことを通知します。Yae Achievement保存ディレクトリにexport-20xxxxxxxxxxxx-seelie.jsonという名前のファイルを見つけてください。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide6.png)

次に、URL：https://seelie.me/settings をクリックし、ウェブサイトにアクセスしてインポートアカウントを選択します。以下の図のように表示されます。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide7.png)

インポートをクリックした後、export-20xxxxxxxxxxxx-seelie.jsonという名前のファイルを選択します。以下の図のように表示されます。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide8.png)

以下の図のようなプロンプトが表示されたら、インポートプロセスは成功です。

![image](https://github.com/prpjzz/yae-markdown-en/blob/main/image/Guide9.png)

この時点で、左側の列のAchievementsを選択して、インポートされた実績データを表示できます。

### 各ツールの紹介については、各ツールの公式ページをご覧ください：

0. [椰羊](https://cocogoat.work/achievement)
1. [胡桃ツールボックス](https://github.com/DGP-Studio/Snap.HuTao)
2. [Paimon.moe](https://paimon.moe/achievement/)
3. [Seelie.me](https://seelie.me/achievements)
4. ~~フォームファイル `.csv`~~
5. [尋空](https://github.com/xunkong/xunkong)
6. [原魔ツールボックス](https://apps.apple.com/app/id1663989619)
7. [TeyvatGuide](https://github.com/BTMuli/TeyvatGuide)
8. [UIAF](https://uigf.org/standards/UIAF.html) JSONファイル
