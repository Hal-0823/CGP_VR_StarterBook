
= VR環境構築

この章では、UnityでVRゲームを開発するための環境構築について説明します。
VRゲームを開発するためには、Unityのインストールと、VRデバイスに対応したSDKのインストールが必要です。以下の手順に従って環境構築を進めてください。

== 構築環境
 * windows 11
 * Meta Quest 3
 * Unity 6000.0.053f
 * Meta XR All-in-One SDK v77.0.0（以降、特別な理由がない限り、単にMeta XR SDKと呼びます）

執筆時点(2026-3-10)では、Meta XR SDK の最新バージョンは v85.0.0 ですが、
後述する Meta XR Simulator の機能が正常に動作しなかったため、v77.0.0 を使用しています。
XR環境は日々アップデートされているため、最新バージョンで動作する可能性もありますが、
安定して動作させるために v77.0.0 を使用することを推奨します。

== Unityのインストールと新規プロジェクトの作成
以下のサイトより Unity 6000.0.053f をダウンロードしてインストールしてください。

 * Unity 6000.0.053f ダウンロードサイト
 ** @<href>{https://unity3d.com/jp/get-unity/download/archive}

Meta3のOSはAndroidであるため、Androidプラットフォームのモジュールを加えておいてください。

インストールが完了したら、新規プロジェクトを作成します。
プロジェクトのテンプレートは「Universal 3D」を選択してください。
新規プロジェクトが立ち上がったら、この節は完了です。(@<img>{01/01})

//image[01/01][新規プロジェクトの画面][scale=0.8]{
//}

== Meta XR All-in-One SDKのインストール
アセットストアから Meta XR SDK をダウンロードしてください。

 * Meta XR All-in-One SDK v77.0.0 ダウンロードサイト
 ** @<href>{https://assetstore.unity.com/packages/tools/integration/meta-xr-all-in-one-sdk-269657}


ダウンロードが完了したら、Add to My Assets をクリックします。(@<img>{01/02})
//image[01/02][Add to My Assetsの画面][scale=0.8]{
//}

UnityEditorが開くので、Version History より v77.0.0 を選択して Install をクリックします。(@<img>{01/03})
//image[01/03][Version Historyの画面][scale=0.8]{
//}

インストールが完了すると、エディターの再起動が求められるので、再起動してください。(@<img>{01/04})
//image[01/04][再起動の画面][scale=0.8]{
//}

これで Meta XR SDK のインストールは完了です。
次の節ではプロジェクトのセットアップ方法を解説します。

== プロジェクトのセットアップ
再起動が完了すると、ハンドスケルトンの更新についてのウィンドウが表示されるので、1番左の Use OpenXR Hand を選択してください。(@<img>{01/05})
//image[01/05][ハンドスケルトンの更新の画面][scale=0.8]{
//}

コンパイルが入るので待機してください。

次に、画面上部の Meta XR Tools から Project Setup Tool を選択します。(@<img>{01/06})
//image[01/06][Project Setup Toolの画面][scale=0.8]{
//}

Project Settings が開き、Project Setup Tool の項目が表示されます。
エラーと警告が大量に表示されるので Fix All をクリックして修正をしましょう。
ただし Fix All をクリックしても修正されないエラーがいくつかあるのですが、ここでは一度無視します。(@<img>{01/07})
同じ手順をAndroidタブでも行っておきます。
//image[01/07][Project Setup Toolのエラーの画面][scale=0.8]{
//}

次に、XR Plugin Management の項目に切り替えます。
初回ではここで Install XR Plugin Management を押下して、インストールをしてください。

インストールが完了したら、Windows、Android両方のタブで OpenXR を有効化してください。(@<img>{01/08})
//image[01/08][XR Plugin Managementの画面][scale=0.8]{
//}

Meta XR Feature Set を有効にするか聞かれるので、Yes を押します。(@<img>{01/09})
//image[01/09][Meta XR Feature Setの画面][scale=0.8]{
//}

すると XR Plugin Management の項目に Project Validation という項目が追加されるので、ここでも Fix All をしておきます。
Windows、Android両方のタブで行ってください。(@<img>{01/10})
//image[01/10][Project Validationの画面][scale=0.8]{
//}

コンパイルを待つと、Windowsタブでは全ての項目が解消されますが、Androidタブではいくつか警告が残ります。
特に問題はないので、ここでは無視します。

===[column] コンパイルが終わらないときは？
ファイルアクセス権限の関係でコンパイルが止まるときがあります。その時はUnity Editor を再起動してください。(@<img>{01/11})
//image[01/11][コンパイルが止まった画面][scale=0.8]{
//}
===[/column]

最後に、Meta3 にビルドするためにプラットフォームをAndroidに変更します。

変更を終えたら再び Project Setup Tool を開き、Recommended Items について Apply All をしてください。(@<img>{01/12})
//image[01/12][Recommended Itemsの画面][scale=0.8]{
//}

これにてプロジェクトのセットアップは完了です。お疲れさまでした。
