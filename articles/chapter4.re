
= ゲーム性の実装 ーメカニクスとイベント

第3章ではオブジェクトを掴むための基本的な設定を学びました。
この章では、掴んだオブジェクトの「トリガーを引く」「ボタンを押す」といった、二次的な操作（Use）と、
それに連動するイベントの設定方法を解説します。

== 「使う」動作の設定
ISDKには、「掴む（Grab）」とは別に、掴んでいる最中にトリガーを指で引き込む動作を検知する
Hand Grab Use という仕組みがあります。

この動作を実装するために、Rootの子要素である ISDKオブジェクトに、Hand Grab Use Interactable コンポーネントを追加します。
//image[04/01][Hand Grab Use Interactableの追加][scale=0.6]{
//}

まずは Use Fingers を設定していきます。
今回、ビームガンのトリガーを引く動作を検知したいので、Index（人差し指）を Required に設定しておきましょう
また、Strength Deadzone を調整することで、どの程度の押し込みでトリガーを発動させるか設定できるため、0.4程度にしておきましょう。
//image[04/02][Use Fingersの設定][scale=0.6]{
//}

また、新たなハンドポーズとして、トリガーを引いた時のポーズを作ります。
もとあった HandGrabPose を複製して、わかりやすいように TriggeredHandGrabPose と名前を変更しておきましょう。
//image[04/03][TriggeredHandGrabPoseの用意][scale=0.6]{
//}

もとのポーズから、人差し指の関節を少し曲げる程度で調整してみましょう。
//image[04/04][TriggeredHandGrabPoseの調整][scale=0.6]{
//}

Hand Grab Use Interactable の Relaxed Hand Grab Pose に、もともと存在した HandGrabPose を、
Tight Hand Grab Pose に先ほど調整した Triggered Hand Grab Pose を指定します。
//image[04/05][Relaxed Hand Grab Pose と Tight Hand Grab Pose の設定][scale=0.6]{
//}

次に、同じく ISDKオブジェクトに、Secondary Interactor Filter コンポーネントを追加します。
そして Primary Interactable にこのオブジェクトの Hand Grab Interactable を指定し、
Secondary Interactable に Hand Grab Use Interactable を指定します。
これにより、掴むインタラクションを1次、使うインタラクションを2次に設定できました。
さらに Select Required を有効にします。
これにより、デフォルトでは1次インタラクティブ要素のホバー時に、2次インタラクションが可能ですが、
Select Required を有効にした影響で、1次要素の選択時にのみ（掴んでいる時のみ）2次インタラクションが可能になります。
//image[04/06][Secondary Interactor Filterの設定][scale=0.6]{
//}

設定が完了したら、Hand Grab Use Interactable の Optionals を開き、Interactor Filters の項目に、先ほど設定した Secondary Interactor Filter を追加しておきましょう。
//image[04/07][Hand Grab Use Interactable の Optionals に追加][scale=0.6]{
//}

これで、使う動作の設定は完了です。

== 「使う」動作の検知
ではこの「使う」動作を検知し、ビームガンのトリガーを引いたときに音を鳴らしてみましょう。

BeamGun.cs というスクリプトを新規作成し、以下のコードを記述します。

//listnum[1][BeamGun.csのコード][C#]{
using UnityEngine;
using Oculus.Interaction;

public class BeamGun : MonoBehaviour, IHandGrabUseDelegate
{
    [SerializeField]
    private AudioSource _audioSource;
    [SerializeField]
    private AudioClip _shootSound;

    public void BeginUse()
    {
        _audioSource.PlayOneShot(_shootSound);
    }

    public void EndUse()
    {
    }

    public float ComputeUseStrength(float strength)
    {
        return strength;
    }
}
//}

このコードで重要なのは、IHandGrabUseDelegate インターフェースを実装している点です。
これにより、Hand Grab Use Interactable のイベントを受け取ることができます。

BeginUse メソッドは、トリガーを引く動作が開始されたときに呼び出されるため、ここで音を鳴らす処理を記述しています。@<br>{}
EndUse メソッドは、トリガーを引く動作が終了したときに呼び出されますが、今回は特に処理は必要ないため空のままにしています。@<br>{}
ComputeUseStrength メソッドは、トリガーの引き具合に応じた強さを計算するためのもので、今回はそのままの値を返すようにしています。@<br>{}

このスクリプトは、ビームガンの Root オブジェクトに追加しておきましょう。

ここで、一つ注意点があります。
通常のカメラなら、AudioSource コンポーネントから音が聞こえますが、
XR環境では Spatial Audio（空間オーディオ）が必要になります。
そのため、Building Blocks から Spatial Audio（@<img>{04/08}） 
をシーンに追加し、Rootオブジェクトの子要素にしておきましょう。
Spatial Audio の詳細については、次の章で説明します。
//image[04/08][Spatial Audio][scale=0.6]{
//}
//image[04/09][Rootの子要素に Spatial Audio を配置する][scale=0.6]{
//}

ここまでできたら、Rootに追加した BeamGunスクリプトに、必要な要素を参照しましょう。
ビームガンの効果音は、フリーの音源などから適当に探してきます。
//image[04/10][Rootに追加したBeamGunスクリプト][scale=0.6]{
//}

これで準備は完了です。実機ビルドをして、ビームガンのトリガーを引いたときに音が鳴るか確認してみましょう。

===[column] トリガーの引き具合を見た目に反映する
ここからは少々応用的な内容になりますが、トリガーの引き具合に応じて、ビームガンの見た目を変化させてみましょう。
前提として、ビームガンなどの3Dモデルのトリガー部分を、シェイプキーで動かせるようにしておく必要があります。

まず、BeamGun.cs で実装した IHandGrabUseDelegate の ComputeUseStrength メソッドを以下のように変更します。
//listnum[2][ComputeUseStrengthの変更][C#]{
private float _useStrength;

public float ComputeUseStrength(float strength)
{
    _useStrength = strength;
    return strength;
}
//}

このように、引数で受け取った strength をクラスのフィールドに保存するようにします。
次に、この _useStrength を受け取って、トリガーの見た目を変える処理を追加します。

//listnum[3][トリガーの見た目を変える処理の追加][C#]{
[SerializeField]
private SkinnedMeshRenderer _triggerRenderer;

public void UpdateTriggerVisual(float strength)
{
    _triggerRenderer.SetBlendShapeWeight(0, strength * 100f);
}
//}

あとは _useStrengthの変更を何らかの方法で検知し、UpdateTriggerVisual メソッドを呼び出せばOKです。

===[/column]

== 発展編
ここからは、ビームガンの実装自体とは関係ありませんが、ISDKの機能について、より深く解説していきます。

=== Interactable Unity Event Wrapper
ISDKには、インタラクションの変化を検知してイベントを発火させる Interactable Unity Event Wrapper というコンポーネントがあります。
これを使えば、例えば掴んだ時、手放した時に特定の処理を呼び出すといったことが簡単に実装できます。

Interactable Unity Event Wrapper を ISDKオブジェクトに追加してみましょう。
//image[04/11][Interactable Unity Event Wrapperの追加][scale=0.6]{
//}

Interactable View には、どの Grab Interactable を対象にするか設定します。
今回は、ISDKオブジェクトについている Hand Grab Interactable を指定しておきましょう。
なお、反対側の手にも同じ機能を実装したい場合、ミラーオブジェクトにも同様にこのコンポーネントを追加しましょう。
//image[04/12][Interactable Viewの設定][scale=0.6]{
//}

このコンポーネントは通常の UnityEvent を購読するのと同じように、インスペクター上でイベントを追加できます。
例えば BeamGun.cs に装備音を鳴らすメソッドなどを用意し、When Select イベントに追加すると、
掴んだときに装備音が鳴るようになります。
//image[04/13][イベント購読][scale=0.6]{
//}

=== Interactor の状態検知
この節では、コントローラーが何かを掴んでいる、など、Interactor自体の状態を検知する手法を説明します。

ISDKオブジェクトに、Interactor Active State コンポーネントを追加します。
//image[04/14][Interactor Active Stateの追加][scale=0.6]{
//}

Interactor フィールドには、状態を検知したい Interactor を指定します。

CameraRig > OVRInteractionComprehensive > Left(Right)Interactions > Interactors のいずれかの Interactor を指定してみましょう。
//image[04/15][Interactorの指定][scale=0.6]{
//}

例えば、Controller and Hand には3つの子要素があり、
//image[04/16][Controller and Handの子要素][scale=0.6]{
//}

ControllerHandGrabInteractor には Hand Grab Interactor がついています。
//image[04/17][ControllerHandGrabInteractorのInspector][scale=0.6]{
//}

これを先ほどの ISDKオブジェクトにつけた InteractorActiveStateの Interactor フィールドに指定してみましょう。
//image[04/18][ControllerHandGrabInteractorを指定][scale=0.6]{
//}

Interactor が設定出来たら、どの状態を検知するか Property から設定します。
例えば、掴んでいる状態を検知する場合は Is Selecting を選択しましょう。
なお、検知する状態は複数選択可能です。
//image[04/19][Propertyの設定][scale=0.6]{
//}

次に、この InteractorActiveState を参照して、実際に掴んだ状態を検知できているか確かめてみます。
ISDK にはデバッグ用に、Active State Debug Visual というコンポーネントがあります。
これを ISDKオブジェクトに追加しましょう。
//image[04/20][Active State Debug Visualの追加][scale=0.6]{
//}

Active State には Interactor Active State を指定します。
Target はこのオブジェクトの Renderer を指定しておきましょう。
カラーについても好みで設定してみてください。
//image[04/21][Active State Debug Visualの設定][scale=0.6]{
//}

これでデバッグの準備は完了です。
以下に実際に実機でデバッグしたときの画像を掲載しています。
//image[Import/06][通常時][scale=0.6]{
//}
//image[Import/07][掴んでいる状態][scale=0.6]{
//}

確かに状態を検知していることが確認できました。@<br>{}
もし正しく動作しなかった場合は、参照が間違っていないか、設定した方の手で掴んでいるかを確認しましょう。@<br>{}

なお、この方法はあくまで Interactor を参照しているため、どの Interactable を掴もうが、
同じ Interactor を参照している限りは反応してしまうので注意してください。
//image[Import/08][何も掴んでいないとき][scale=0.6]{
//}
//image[Import/09][片方のオブジェクトを掴むと、もう片方も反応する][scale=0.6]{
//}

片方の手だけでなく、両手のアクティブ状態をまとめて検知するには、
Active State Group コンポーネントを使用する。
手順は省略するが、反対側の ISDKオブジェクトにも同じように Interactor Active State を設定し、
Rootオブジェクトに ActiveStateGroup コンポーネントを追加する。
//image[04/22][Active State Groupの追加][scale=0.6]{
//}

Active States に 両方の ISDKオブジェクトの Interactor Active State を追加し、Logic Operator を設定します。
AND ならどちらも満たした時、OR なら少なくともどちらかが満たした時、XOR ならどちらか一方が満たした時に、Active State Group がアクティブになります。
//image[04/23][Active State Groupの設定][scale=0.6]{
//}

これを先ほどのデバッグコンポーネントに参照させれば動作確認が可能です。
//image[04/24][Active State Groupを参照する][scale=0.6]{
//}

===[column] 自作したスクリプトで ActiveState を参照する
Active State Debug Visual コンポーネントではなく、
実際に自作のスクリプトで Active State の Active 状態を検知する方法です。

//listnum[4][Active State を参照するコード例][C#]{
using UnityEngine;
using Oculus.Interaction;

/// <summary>
/// コントローラーのActive状態(ActiveState)を取得して、
/// 状態に応じた処理を行うサンプル。
/// </summary>
public class ActiveStateSample : MonoBehaviour
{
    [SerializeField, Interface(typeof(IActiveState))]
    private Object _activeState;
    private IActiveState ActiveState { get; set; }

    private void Start()
    {
        ActiveState = _activeState as IActiveState;
    }

    private void Update()
    {
        bool isActive = ActiveState.Active;

        if (isActive)
        {
            Debug.Log("Active");
        }
        else
        {
            Debug.Log("Inactive");
        }
    }
}
//}

===[/column]

===[column] インスタンス化した時に Interactor を参照する方法
ISDKオブジェクトを含むオブジェクトがプレハブ化されていて、Instantiate して生成する場合、
事前に Inspector 上で Interactor を参照しておくことができません。
//image[04/25][プレハブのInspector][scale=0.6]{
//}

この場合の対策として、InteractorActiveState.cs の最後に以下のようなコードがあります。
//image[04/26][InteractorActiveState.csのコード（一部抜粋）][scale=0.6]{
//}

これらのメソッドを利用すれば、Inject する形で Interactor を参照することができます。
詳細な手順は割愛しますが、状況に合わせて活用してみてください。

===[/column]

== Interactable の状態検知
前節では Interactor の状態を検知する方法を説明しましたが、Interactable の状態を検知することも可能です。
実装において特別なコンポーネントはなく、以下のようなコードを書く必要があります。
//listnum[5][Interactableの状態を検知するコード例][C#]{
using Oculus.Interaction;
using UnityEngine;

/// <summary>
/// Interactableの状態を取得して、状態に応じた処理を行うサンプル。
/// </summary>
public class InteractableStateSample : MonoBehaviour
{
    [SerializeField, Interface(typeof(IInteractableView))]
    private Object _interactableView;
    private IInteractableView InteractableView { get; set; }

    private void Awake()
    {
        InteractableView = _interactableView as IInteractableView;
    }

    private void OnEnable()
    {
        InteractableView.WhenStateChanged += HandleStateChanged;
    }

    private void OnDisable()
    {
        InteractableView.WhenStateChanged -= HandleStateChanged;
    }

    private void HandleStateChanged(InteractableStateChangeArgs args)
    {
        switch (InteractableView.State)
        {
            case InteractableState.Normal:
                Debug.Log("Normal");
                break;
            case InteractableState.Hover:
                Debug.Log("Hover");
                break;
            case InteractableState.Select:
                Debug.Log("Select");
                break;
            case InteractableState.Disabled:
                Debug.Log("Disabled");
                break;
        }
    }
}
//}

このコードでは、IInteractableView インターフェースを実装したオブジェクトの状態変化を検知しています。
ここで、IInteractableView を実装したクラスとは、Hand Grab Interactalble などのことです。
このインターフェースでは、Interactable の状態変化を検知し、WhenStateChanged イベントを発火させる仕組みが用意されているため、
これを購読する形で状態変化を検知できます。
なお、前述した Interactable Unity Event Wrapper コンポーネントを使用すれば、
コードを書かずに状態変化を検知することも可能です。
