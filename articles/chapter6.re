
= Haptics

VRにおいて、触覚フィードバックは非常に重要な要素の一つです。
この章では、Meta XR SDK における触覚フィードバック（Haptic）の基本的な使い方を説明していきます。

== Haptic Studio
Haptic を準備するには、HapticsStudio が必要になります。@<br>{}
以下のサイトからダウンロードしてください。（使用バージョン：2.2.0）
@<href>{https://developers.meta.com/horizon/downloads/package/meta-haptics-studio-win}

Meta3 にも Haptics Studio をインストールしましょう。
//image[Import/11][Meta3 での Haptics Studioのインストール][scale=0.6]{
//}

起動するとこのような画面が表示されます。
//image[Import/12][Haptics Studioの起動画面][scale=0.6]{
//}

このアプリ単体でもサンプルを再生して、Haptic を体験することができます。
//image[Import/13][Haptics Studioのサンプル再生][scale=0.6]{
//}

続いて、PC と Meta3 を連携して、Haptic を作成する方法を説明します。
PC と Meta3 をUSBで接続し、Meta3 側で Connect を押してください。
これで PC と Meta3 の接続が完了します。
//image[Import/14][PCとMeta3の接続][scale=0.6]{
//}

PC側での操作に移ります。
Haptics Studio の左上のメニューから、New Project を選択して新規プロジェクトを作成します。
//image[06/03][新規プロジェクトの作成][scale=0.6]{
//}

制作をはじめるには2種類の方法があります。@<br>{}
 * Design From Audio：好きな音源を取り込み、その音源から Haptic を生成する。
 * Design Freeform Haptics：波形を直接編集して Haptic を生成する。

//image[06/04][Hapticの制作方法の選択][scale=0.6]{
//}

今回は、Design From Audio を選択して、ビームガンの発射音を収録した「ビームガン.mp3」を読み込みました。
//image[06/05][音源の読み込み][scale=0.6]{
//}

各要素について簡単に説明します。
 * Amplitude：振幅。Haptic の強さを表すパラメーター。
 * Frequency：周波数。Haptic の振動の速さを表すパラメーター。

 * Gain：入力音量。Haptic 全体の強さを調整するパラメーター。
 * Resolution：解像度：Haptic の振動の細かさを調整するパラメーター。

 * Emphasis：強調ポイント。Haptic の中で特に強調したい部分を設定できる。
 * Sensitivity：感度。音声データの素早い変化から、強調ポイントをどれくらい抽出するかを調整するパラメーター。
 * EnvelopeReduction：強調ポイントの振幅を調整する。0%で強調ポイントと振幅が一致する。逆に100%に近づくにつれ、強調ポイントの振幅は0に近づく。強調ポイント自身の振幅が小さくなるわけではないため、このパラメータを小さくすると強調ポイントが目立ちづらくなり、逆に大きくすると目立つようになる。

//image[06/06][Envelope Reduction 0%][scale=0.6]{
//}

//image[06/07][Envelope Reduction 100%][scale=0.6]{
//}

すでに設置されているポイントを編集する場合は Selection Tool を選択。
//image[06/08][Selection Toolの選択][scale=0.6]{
//}

新しいポイントを追加する場合は Pen Tool を選択。
//image[06/09][Pen Toolの選択][scale=0.6]{
//}

強調ポイントを手動で追加する場合は Emphasis Tool を選択。
//image[06/10][Emphasis Toolの選択][scale=0.6]{
//}

完成した Haptic は、Export から .haptic ファイルとして出力できます。
それ以外の形式は Unity では利用できません。
//image[06/11][Hapticの出力][scale=0.6]{
//}

== Haptic の再生
前節で作成した .haptic ファイルを Unity にインポートして、実際に再生してみましょう。
インポートした .haptic ファイルは、Scriptable Object として扱われます。
//image[06/12][インポートした .haptic ファイル][scale=0.6]{
//}

この Haptic を再生するには、Haptic Clip Player をインスタンス化し、Haptic Clip として渡せばよいです。

//listnum[6][Haptic Clip Playerのコード例][C#]{
using UnityEngine;
using Oculus.Haptics;

/// <summary>
/// HapticClipを再生するサンプル。
/// </summary>
public class HapticPlaySample : MonoBehaviour
{
    [SerializeField]
    private HapticClip _hapticClip;

    private HapticClipPlayer _hapticClipPlayer;

    private void Awake()
    {
        _hapticClipPlayer = new HapticClipPlayer();
    }

    public void PlayHaptic(Controller controller)
    {
        _hapticClipPlayer.clip = _hapticClip;
        _hapticClipPlayer.Play(controller);
    }
}
//}

ほとんど AudioClip を再生するのと同じような感覚で Haptic Clip を再生できることがわかりますね。
HapticClipPlayer の Play メソッドの引数には、どちらのコントローラーで Haptic を再生するかを指定します。

//listnum[7][Controllerの定義][C#]{
public enum Controller
{
    Left,
    Right,
    Both
}
//}
