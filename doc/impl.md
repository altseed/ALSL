#ALSL実装

## 基本的な使い方
 `alsl -o filename filename.alsl`
 
 -oオプションに拡張子なしのファイル名を指定すると、HLSL向けの.fxファイルとGLSL向けの.fsファイルが出力されます

## コマンドライン引数

## 変換される型
 + bool2
 + bool3
 + bool4
 + int2
 + int3
 + int4
 + float2
 + float3
 + float4
 + mat2
 + mat3
 + mat4

行列はHLSLではrow_major修飾されます


## 変換される関数
 + mul(m1, m2): 行列の積
