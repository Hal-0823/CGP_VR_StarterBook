
= VR空間とAudio / UI

この章ではAudioやUIなどを、VR空間に配置する方法について解説します。
VR空間に配置することで、ユーザーはより没入感のある体験を得ることができます。

== VR空間上で音を鳴らす
VR空間上では、音を鳴らすために Spatial Audio（空間オーディオ）を使用する必要があります。
Spatial Audio を使用することで、ユーザーの位置や向きに応じて音の聞こえ方が変化し、よりリアルな音響体験を提供できます。

まずは Building Blocks から Spatial Audio をシーンに追加しましょう。
//image[05/01][Spatial Audioの追加][scale=0.8]{
//}

今回はBGMとして、適当なフリー素材を使用します。
なお、Spatial Audio として使用する音源は、Force To Mono を有効化して、
モノラルに変換しておく必要があるので注意してください。
//image[05/02][音源の設定][scale=0.8]{
//}

Spatial Audio の Audio Source に、先ほどの音源を指定します。
なお、Spatial Audio の Spatial Blend は、完全に3D空間で鳴らしたい場合は1に設定してください。
一方でUIの効果音など、ユーザーの位置に関わらず聞こえるようにしたい場合は0に設定しましょう。
//image[05/03][Audio Sourceの設定][scale=0.8]{
//}

Audio Source の Play On Awake を有効化しておけば、シーン開始と同時にBGMが流れ始めます。
試しに実機で確認してみてください。

== UIの設置
XR空間では、特殊なコントローラーに対応したポインタブルなキャンバスを作ることで、
ボタンクリックなどのインタラクションを実現します。
インタラクションが必要ない場合、Canvas の Render Mode を World Space に変更するだけで
VR空間上に表示可能になります。

ここでは、インタラクト可能なUIを実装する方法を説明していきます。@<br>{}
まずはキャンバスを設置し、Render Mode を World Space に変更します。
//image[05/04][キャンバスの設置][scale=0.8]{
//}

そのままのサイズだとワールド空間に対して大きすぎるので、
Scale を0.001倍程度に縮小しておきましょう。
//image[05/05][キャンバスの縮小][scale=0.8]{
//}

続いて、キャンバスに Pointable Canvas コンポーネントを追加します。
対象のキャンバスには、自身の Canvas コンポーネントを指定しておきましょう。
//image[05/06][Pointable Canvasの追加][scale=0.8]{
//}

続いてキャンバスに当たり判定をつけていきます。
キャンバスに空の子要素を追加し、Collider とします。
//image[05/07][Colliderの追加][scale=0.8]{
//}

この子要素には Box Collider を追加しますが、
この時点だと、 Canvas とのスケーリングの差でコライダーが見えないほど小さいため、
サイズを大きくしておきましょう。
大きさはだいたいキャンバスを覆う程度であれば問題ありません。
//image[05/08][Colliderのサイズ調整][scale=0.8]{
//}

Collider にはさらに Collider Surface コンポーネントを追加し、自身を Collider に割り当てます。
//image[05/09][Collider Surfaceの追加][scale=0.8]{
//}

続いて同じく、キャンバスに空の子要素を追加し、Surface と名前をつけます。
//image[05/10][Surfaceの追加][scale=0.8]{
//}

このオブジェクトに対し、Plane Surface コンポーネントを追加します。
//image[05/11][Plane Surfaceの追加][scale=0.8]{
//}

このコンポーネントが追加出来たら、
Canvas に RayInteractable コンポーネントを追加しましょう。
参照設定は以下です。
 * Pointable Element -> Canvasオブジェクト
 * Surface -> Colliderオブジェクト
 * Select Surface -> Surfaceオブジェクト

//image[05/12][RayInteractableの設定][scale=0.8]{
//}

これにてキャンバスの設定は完了しました。
プレハブ化しておけば、他のUIを作るときも同じ設定をする必要がなくなります。

次に Event System の設定に移ります。
イベントシステムにデフォルトで含まれる Input System UI Input Module を削除し、
代わりに Pointable Canvas Module を追加します。
//image[05/13][Event Systemの設定][scale=0.8]{
//}

あとは通常通り、キャンバスにパネルやボタンなどの要素を追加してみましょう。
//image[05/14][UI要素の追加][scale=0.8]{
//}

この状態でビルドし、実機で確認すると、
@<img>{Import/10}のように、コントローラーから Ray が伸びて、キャンバスの要素を選択できます。
//image[Import/10][UIの動作確認][scale=0.8]{
//}

XR Simulator でも動作確認可能である。
//image[05/15][XR Simulatorでの動作確認][scale=0.8]{
//}
