## 使用説明書

1. YaeAchievement（最新バージョン）をダウンロード：

こちらをクリック：https://github.com/HolographicHat/YaeAchievement/releases

赤枠で囲まれた「YaeAchievement.exe」という名前のファイルをクリックすると、自動的にポップアップしてダウンロードされます。このファイルをデスクトップや他の見やすいフォルダに保存することをお勧めします。

![Guide1](https://github.com/user-attachments/assets/dbe32d1f-3a73-4948-b854-1fb6151ad7f3)

2. .NET Runtime 7をインストール（ランタイムが既にインストールされている場合はこのステップを無視できます）

こちらをクリック：https://dotnet.microsoft.com/en-us/download/dotnet/thank-you/runtime-8.0.4-windows-x64-installer .

または、Windows 11を使用しているか、Wingetがインストールされている場合は、`winget install Microsoft.DotNet.Runtime.7`を実行します。

ウェブページにアクセスすると、ブラウザが自動的にポップアップしてダウンロードされます。ファイルをデスクトップや他の見やすいフォルダに保存します。

ダウンロードが完了したら、dotnet-runtime-x.x.x-win-x64.exeという名前のファイルを開くと、インストールウィンドウがポップアップします。以下の図のように表示されます。

![Guide2](https://github.com/user-attachments/assets/35f421af-dd45-41ea-94f9-e3cf90710f0f)

インストールをクリックするだけです。

3. メインプログラムを開くための操作と実績エクスポートのオプション

最初のステップでダウンロードした「YaeAchievement.exe」という名前のファイルをダブルクリックして開くと、原神が起動していることを示します。以下の図のように表示されます。

![Guide3](https://github.com/user-attachments/assets/c3375188-1fa3-4a0b-9007-358afbcaae91)

原神の起動が完了したら、ゲームに入ります。

ゲームに入ると、ツールがどのツールにエクスポートするかを選択するように促します。以下の図のように表示されます。

![Guide4](https://github.com/user-attachments/assets/c806582a-3608-4c86-af26-ce8e631ff610)

グローバルユーザーの場合、[3] Seelie.meまたは[4] csvファイルにエクスポートを選択する必要があります。

選択後、各ページは次のようにツールをエクスポートします：

#### Snap.Hutao：

![Guide5](https://github.com/user-attachments/assets/40d547d8-fe04-4462-8b78-284394a44c36)

#### Seelie.me：

この時点で、Yae Achievementは実績データがエクスポートされたことを通知します。Yae Achievement保存ディレクトリにexport-20xxxxxxxxxxxx-seelie.jsonという名前のファイルを見つけてください。

![Guide6](https://github.com/user-attachments/assets/91cdb0e6-883d-4f5e-9f1f-416eb8c16433)

次に、URL：https://seelie.me/settings をクリックし、ウェブサイトにアクセスしてインポートアカウントを選択します。以下の図のように表示されます。

![Guide7](https://github.com/user-attachments/assets/e6a9ddb1-b075-4f0b-9e42-a1d61b4808bc)

インポートをクリックした後、export-20xxxxxxxxxxxx-seelie.jsonという名前のファイルを選択します。以下の図のように表示されます。

![Guide8](https://github.com/user-attachments/assets/1b7edb51-ff0d-415c-bd96-0e6c9ac7a238)

以下の図のようなプロンプトが表示されたら、インポートプロセスは成功です。

![Guide9](https://github.com/user-attachments/assets/e155b4e5-ce15-4dd8-9633-a83b9759bce1)

この時点で、左側の列のAchievementsを選択して、インポートされた実績データを表示できます。

### 各ツールの紹介については、各ツールの公式ページをご覧ください：

0. [椰羊](https://cocogoat.work/achievement)
1. [胡桃ツールボックス](https://github.com/DGP-Studio/Snap.HuTao)
2. [Paimon.moe](https://paimon.moe/achievement/)
3. [Seelie.me](https://seelie.me/achievements)
4. [フォームファイル `.csv`](https://ja.wikipedia.org/wiki/Comma-separated_values)
5. [尋空](https://github.com/xunkong/xunkong)
6. [原魔ツールボックス](https://apps.apple.com/app/id1663989619)
7. [TeyvatGuide](https://github.com/BTMuli/TeyvatGuide)
8. [UIAF](https://uigf.org/standards/UIAF.html) JSONファイル
