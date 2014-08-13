---
title: ALSL実装
date: 2014/07/12
---

#ALSL実装

## 基本的な使い方
 `alsl -o filename filename.alsl`
 
 -oオプションに拡張子なしのファイル名を指定すると、HLSL向けの.fxファイルとGLSL向けの.fsファイルが出力されます

## コマンドライン引数

## 変換される型

|ALSL|HLSL|GLSL|
|---:|---:|---:|
|bool2|bool2|bvec2|
|bool3|bool3
|bool4|bool4
|int|int
|int2|int2
|int3|int3
|int4|int4
|uint|uint
|uint2|uint2
|uint3|uint3
|uint4|uint4
|float|float
|float2|float2
|float3|float3
|float4|float4
|float22|row_major float2x2
|float33|row_major float3x3
|float44|row_major float4x4
|Texture2D|Texture2D
|Sampler|SamplerState

## 変換される関数
 + mul(m1, m2): 行列の積
 + GetTextureSize(texture, sampler)
 + SampleTexture(texture, sampler, uv)

## その他の仕様
 + 三項演算子は使用不可
 + 変数宣言やメンバ宣言は一行に一つ(型名一つに変数名をカンマで区切って複数宣言できない)
 + `struct{...} var;`のような構造体定義の直後に変数を宣言することはできない
 + 構造体定義の後のセミコロンは省略できる
 + 定数宣言でのconstは型名の前にしか記述できない
