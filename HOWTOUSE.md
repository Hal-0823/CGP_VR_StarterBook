# 執筆方法について
執筆者 CGP ヨシオカ

VSCodeでの執筆環境を整えるためにやったことの備忘録です。
参考にさせていただいた記事
https://zenn.dev/posita33/articles/pub_review_techbook_16#2024%2F04%2F13-re%3Aview%E7%92%B0%E5%A2%83%E3%82%92%E6%A7%8B%E7%AF%89%E3%81%97%E3%82%88%E3%81%86

## 必要なもの
Docker Desktop
GitHub Desktop

## やりかた

まず記事の内容に沿って Docker Desktop for Windowsをダウンロード。
ばーっとインストールやセットアップをやっている最中に、
https://github.com/TechBooster/ReVIEW-Template
から Use this template を押して Create a new repository をする。
リポジトリを作ったら、GitHub Desktopでクローンしてローカルに落とす。

VSCodeでリポジトリを開いたら、
https://marketplace.visualstudio.com/items?itemName=atsushieno.language-review
の拡張機能をインストールしておこう。
PDF閲覧用の vscode-pdf もインストールする。

## PDF出力
まず articlesフォルダに docker-compose.yaml を作り以下を記述
```yaml
version: '3'
services:
  review:
    image: vvakame/review:5.8
    volumes:
      - .:/(リポジトリ名)
    build: .
    working_dir: /(リポジトリ名)
```

次に articles/lib に review.rake があるので、そこに以下を追加
```rake
# 追加するpublishタスク
task :cgp_publish => [:pdf] do
  sh "cp CGP.pdf vscode_viewer.pdf"
end
```

最後に リポジトリのフォルダ直下に .vscode/tasks.json を作成。
以下を記述。
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Re:VIEW Build PDF",
            "type": "shell",
            "command": "docker run --rm -v ${workspaceFolder}:/work vvakame/review /bin/bash -c 'cd /work/articles && rake cgp_publish'",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": []
        }
    ]
}
```

以上で準備は完了。
VSCodeで Ctrl + Shift + B を押すとタスクが実行され、
CGP.pdf と vscode_viewer.pdf の2種類が発行される。
両者は内容は同じだが、CGP.pdf は次にこのタスクを実行した際、一時的に削除され、再発行される。このとき、画面の右などで記事の見た目確認用に開いていても、強制的に閉じられてしまう。その対策として、vscode_viewer.pdf を開いておけば、CGP.pdf が発行されたあと、その内容をまるっとコピーしてくるだけなので、閉じられたりはしない。