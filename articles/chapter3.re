
= VRインタラクションの基礎

VRゲームにおける最も基本的かつ重要な体験は、仮想空間にあるオブジェクトを「掴む（Grab）」という動作です。
この章では Meta XR SDK に含まれる、Meta XR Interaction SDK（以降 ISDK）を用いて、物理的な違和感のないインタラクションを実装する手法を解説します。

== 「掴む」動作とGrabbableの概念
VRにおけるインタラクションは、Interactor（干渉するもの：手やコントローラー）と
Interactable（干渉されるもの：オブジェクト）の組み合わせで成立します。

ISDKにおいて、「掴む」を実現する中心的なコンポーネントが Grabbable です。
これは、オブジェクトが掴まれるための機能を提供するコンポーネントで、掴まれた際の挙動や物理的な反応を管理します。
よって、掴む対象のオブジェクトには、必ず Grabbable コンポーネントを追加する必要があります。

== 掴めるオブジェクトの最小構成
オブジェクトを掴めるようにするためには、以下の4つの要素が必須となります。

 1. Rigidbody：物理挙動を制御します。VRで掴む対象には、勝手に落下しないよう Is Kinematic を状況に応じて使い分けます。
 2. Collider：当たり判定です。掴む対象の形状に合わせて適切な Collider を選択します。
 3. Grabbable：掴むロジックのコアです。
 4. Grab Interactable：掴むための具体的な設定を行うコンポーネントです。

== 実践：ビームガンの実装手順
サンプルゲームの主役であるビームガンを例に、掴む動作の実装手順を解説します。
あなたが作りたいオブジェクトに合わせて、適宜調整してみてください。
//image[03/01][ビームガンのモデル][scale=0.8]{
//}

はじめに、完成したビームガンの構成は@<img>{03/02}のようになります。
//image[03/02][ビームガンのHierarchy][scale=0.8]{
//}

要素の説明は以下の通りです。

 * Root：ビームガン全体の親オブジェクト
 ** Visual：3Dモデル
 ** Colliders：当たり判定用
 ** ISDK_GrabInteraction：ISDKのGrab Interactableコンポーネントを設定

それでは、順を追って実装していきましょう。
まずは Empty Object を作成して、Root とします。
Rootには Rigidbody、Grabbable、Grab Free Tarnsformer の3つのコンポーネントを追加します。

@<img>{03/03}を参考に、各コンポーネントの設定も行います。
Rigidbody の Is Kinematic を有効にしておくと、勝手に落下することがなくなります。
Grabbable の Rigidbody には Root の Rigidbody を指定しておきましょう。
また、One Grab Transformer には Root の Grab Free Transformer を指定しておきます。

ここで Grab Transformer とは、掴んだオブジェクトの位置や回転をどのように制御するかを定義するコンポーネントです。
今回追加した Grab Free Transformer は、Interactable が Interactor に完全に追従するような挙動を実現します。
この設定を、Grabbable の One Grab Transformer（片手で掴む場合の挙動）に指定することで、ビームガンを片手で掴んだ際、完全に追従するようになります。
//image[03/03][Rootのコンポーネント][scale=0.8]{
//}

つづいて、Rootに空の子オブジェクトを2つ追加し、Visual、Colliders とします。
この Visual には特別追加するコンポーネントはありません。
子要素として、ビームガンの3Dモデルを配置しておきましょう。

Colliders には、ビームガンの当たり判定用の Collider を配置します。
@<img>{03/04}のように、Colliders 自身に複数の Collider を追加してもよいですし、
子要素に複数の Collider 用オブジェクト を配置しても構いません。
//image[03/04][Collidersの構成][scale=0.6]{
//}

@<img>{03/05}のようになるべく単純な形状の Collider を配置することで、物理挙動が安定しやすくなります。
//image[03/05][Colliderの配置例][scale=0.6]{
//}

または@<img>{03/06}のように、単一の Mesh Collider を追加して Convex を有効にすることで、簡単に複雑な形状の当たり判定を作成することもできます。
//image[03/06][Mesh Colliderの例][scale=0.6]{
//}

最後に Root に ISDK_GrabInteraction を追加します。

Root オブジェクトを右クリック > Interaction SDK > Add Grab Interaction で追加できます。
//image[03/07][Grab Interactionの追加][scale=0.6]{
//}

@<img>{03/08}のように、Pointable Element、Rigidbody、Grab Source にはそれぞれ Root オブジェクトを参照しておきます。
//image[03/08][Grab Interactionの設定][scale=0.6]{
//}

これにて最小限の構成は完了です。
XR Simulator や 実機ビルドをして、実際に掴む動作をテストしてみましょう。

== ハンドポーズとミラーリング
前節で実装した掴む動作をより自然にするには、ハンドポーズの設定が必要不可欠です。
ISDK_HandGrabInteraction の Hand Grab Interactable コンポーネントにある、Add HandGrabPose Key with Scale 1.00 をクリックして、
ハンドポーズを追加してみましょう。
//image[03/09][ハンドポーズの追加][scale=0.6]{
//}

すると ISDK_HandGrabInteraction の子要素に HandPose が追加されます。
//image[03/10][HandPoseの追加][scale=0.6]{
//}

初期ポーズは@<img>{03/11}のような、手を開いた状態になっています。
//image[03/11][初期ポーズ][scale=0.6]{
//}

ビームガンを掴んでいるハンドポーズを想像しながら、
まずはTransform を調整して、大まかな位置と回転を合わせましょう。
//image[03/12][大まかな調整][scale=0.6]{
//}

シーン上で、ハンドポーズオブジェクトの各関節部分にあるリングをマウスで回転させることで、
関節を回転させることができます。
//image[03/13][関節の調整][scale=0.6]{
//}

オブジェクトと手が重なって見えづらい場合は、適宜ハンドポーズオブジェクトをスライドして調整してもいいでしょう。
//image[03/14][ハンドポーズオブジェクトのスライド][scale=0.6]{
//}

各指を調整して、自分の想像したハンドポーズになれば完成です。
今回のビームガンを掴むハンドポーズは、@<img>{03/15}のようになりました。
//image[03/15][ビームガンを掴むハンドポーズ][scale=0.6]{
//}

===[column] ハンドポーズを調整するコツ
ハンドポーズの調整は手作業で行うため、感覚を頼りにする部分が大きいです。
調整のコツとして以下があげられます。

 * 現実で自分の手や資料を参考にしながら、模写のように調整する。
 * はじめはおおまかな手の位置を合わせたり、根本の関節から動かしてみるなど、大きな動きのある部分から調整する。

はじめから細部にこだわりすぎず、まずは全体のバランスを見ながら作業を進めてみましょう。

===[/column]

完成したハンドポーズは、反対の手にもミラーリングして適用することができます。
ISDK_HandGrabInteraction の Hand Grab Interactable コンポーネントにある、Create Mirrored HandGrabInteractable を押してみましょう。
//image[03/16][ミラーリングの実行][scale=0.6]{
//}

すると Root オブジェクトの子要素に、ISDK のミラーが追加されます。
これで、面倒な調整をスキップして反対側の手についてもハンドポーズが作成できました。
//image[03/17][ミラーオブジェクト][scale=0.6]{
//}

===[column] ハンドポーズを設定した場合//設定しない場合の違い
ハンドポーズを設定の有無で、掴む動作について比較を行った。(@<img>{Import/01})
左がハンドポーズを設定したオブジェクトで、右がハンドポーズを設定していないオブジェクトを掴んでいる様子である。
左はコライダーの範囲内でジェスチャーをすると、手が自然にオブジェクトにフィットしているのに対し、右はコライダーがついているところならどこでも自由に掴めてしまう。
//image[Import/01][ハンドポーズの有無の比較][scale=0.6]{
//}

===[/column]

== 掴み方のルールと挙動の設定
ISDK では、掴み方のスタイルを詳細に設定できます。

=== Fingers Freedom
Hand Grab Pose の Fingers Freedom では、各指に対して自由度を設定できます。
これはハンドポーズをとった際、指がどの程度自由に動くかを定義するもので、以下の3つのモードがあります。

 * Free：指は自由に動きます。
 * Constrained：指はポーズの位置を超えて動くことはありません。
 * Locked：指はポーズの位置に完全に固定されます。

//image[03/18][Fingers Freedomの設定][scale=0.6]{
//}

=== Supported Grab Types
Hand Grab Interactable の Supported Grab Types には、どの掴み方に対応するかを設定できます。

 * Pinch：親指と人差し指をくっつけるような掴み方で、例えばチェスの駒をつまむときに適しています。
 * Palm：手のひら全体で掴むスタイルで、今回のビームガンのようなオブジェクトを掴むときに適しています。

//image[03/19][Supported Grab Typesの設定][scale=0.6]{
//}

=== Palm Grab Rules
Hand Grab Interactable の Palm Grab Rules では、どの指がポーズ（ジェスチャー）と一致していたらグラブをトリガーするか決定できます。
項目は以下のいずれかです。

 * Required（必須）
 * Optional（任意）
 * Ignored（無視）

グラブ動作がトリガーされるには、すべての Required な指がジェスチャーを実行しなければなりません。
Required マークが一つもない場合、少なくとも一つの Optional な指がジェスチャーを実行しなければなりません。
Ignored な指は、グラブ動作のトリガーにおいて完全に無視されます。

//image[03/20][Palm Grab Rules の設定][scale=0.6]{
//}

=== Hand Alignment
Hand Grab Interactable の Hand Alignment の項目は、画面上の手がインタラクトに合わせてどのように動くかを設定できます。

 * AlignOnGrab：この動作は、グラブ開始時点で手をInteractableに視覚的に自動スナップさせます。これがデフォルトの動作です。
 * AttractOnHover：これは、ホバリングが強くなるにつれて、最終的なハンドポーズを段階的に適用します。
 * AlignFingersOnHover：これは、ホバリング中にユーザーが手を閉じ始めると、指の最終ポーズを段階的に適用します。
 * None：画面上の手にハンドポーズは視覚的に適用されません。

//image[03/21][Hand Alignment の設定][scale=0.6]{
//}

== Meta XR Simulator で確認する
第2章で解説した Meta XR Simulator を活用して、ビームガンを掴む動作の検証をしましょう。
XR Simulator を有効にして、シーンを再生してください。

キーボードの2または4で掴み、3でつまむことができます。（左クリックまたはキーボードの1で解除）
前述した、Supported Grab Types が Palm だと、3を押してもつまむことができません。
//image[03/22][Pinch動作では掴めないビームガンの様子][scale=0.6]{
//}
//image[03/23][Palm動作でビームガンを掴んでいる様子][scale=0.6]{
//}

Inputs の Active Inputs を Hand(L) にすることで、左手のみの精密な操作に切り替えられます。
また、Left Input や Right Input が Hand ではなく Controller になっていると、Hand Grab Interactable をつけたオブジェクトは反応しないので
注意しましょう。
//image[03/24][Active Inputsの画面][scale=0.7]{
//}

この状態で手の姿勢を操作して、最終的な見栄えを確認しましょう。
//image[03/25][最終的な姿勢][scale=0.6]{
//}

===[column] 掴んでいないのにハンドポーズが表示される
XR Simulator で確認した際、掴んでいないのに@<img>{03/26}のようにシミュレーション画面にハンドポーズが表示されることがあります。
この原因は、Hierarchy上で ISDK_HandGrabInteraction を選択していることです。
理想的な状態のハンドポーズを確認する手段として使ってみてもいいでしょう。
//image[03/26][ISDK_HandGrabInteractionを選択している状態][scale=0.6]{
//}

===[/column]

== 手とコントローラーの違い
Meta Quest 3 では、手のトラッキングとコントローラーの両方を使用してインタラクションが可能です。
両者の性質は、入力方法（手のジェスチャー vs ボタン操作）や、触覚フィードバックの有無、精度など、様々な点で異なります。
ISDK では、これらの違いを考慮し、「Hand Grab」と「（Controller）Grab」の別々のInteractableとして、同じオブジェクトに持たせることができます。
これにより、ユーザーは手でもコントローラーでも自然にオブジェクトを掴むことができるようになります。

ところで、コントローラーを使用しながらも、コントローラーがどのような振る舞いをするかを設定できる項目が存在します。

Building Block で追加した Camera Rig についている OVR Manager を見てみましょう。
OVR Manager の項目の中に、Controller Driven Hand Poses Type という項目があります。
//image[03/27][Controller Driven Hand Poses Type][scale=0.6]{
//}

この項目を設定することで、Controller がどのように駆動するかを設定できます。

 * None：常にコントローラーが表示される。掴み判定はコントローラー（Grab Interactable）
//image[Import/02][None][scale=0.6]{
//}

 * Conforming to Controller：通常時は@<img>{Import/03}のように、コントローラーを持った手が表示される。物を掴むと@<img>{Import/04}のように手だけになる。掴み判定は手（Hand Grab Interactable）
//image[Import/03][Conforming to Controller（通常時）][scale=0.6]{
//}
//image[Import/04][Conforming to Controller（掴んだとき）][scale=0.6]{
//}

 * Natural：常に手が表示される。掴み判定は手（Hand Grab Interactable）
//image[Import/05][Natural][scale=0.6]{
//}

このように、コントローラーで掴んだ際の判定をコントローラーにするか、手にするかを選択できます。
この資料では Conforming to Controller を使い、Hand Grab Interactable をコントローラーでも掴めるようにします。

