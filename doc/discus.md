# 2014/06/01
 * 外部から値を設定する文法はUnityを真似る
 * mainを複数書くためのサポートは追加しない
 * include相当文はプロトタイプ宣言の必要性を調べてから。キャッシュがないので
   言語側のincludeを使わずに処理系側で一つのファイルにまとめた方が良いかも
 * 属性の用途は現状、MaterialType、両面片面、アルファブレンドタイプ+α程度
 * シェーダ間等の受け渡しは要検討、セマンティクスなしの構造体にしておいて
   HLSLではセマンティクスを追加等? いずれにせよmainを改変するかラップするかの
   必要あり
