## 自分用スタイルガイド

### 命名規則

- 変数、名前空間 : "snake_case"

- テンプレート変数、メンバ変数 : "snake_case" + "_"

- クラス、型、関数、メソッド : "PascalCase"

- 以下は例外的に : "snake_case"
`begin / end`, `cbegin / cend`, `rbegin / rend`, `size`, `data`, `empty`, `clear`, `insert`, `erase`, `what`, `get_*`, `cget_*`, `set_*`, `to_*`


### クラス宣言の順序

- テンプレートに関わる制約

- 型エイリアス、クラス内クラスの前方宣言

- static メンバ変数

- メンバ変数

- static メソッド

- コンストラクタ、デストラクタ、コピー・ムーブコンストラクタ

- 簡単なpublicメソッド

- public operator

- その他のメソッド

- その他


### 一行の長さ

- 文字リテラルを除き、一行を80字以内におさめる。
