---
title: ALSL����
date: 2014/07/12
---

#ALSL����

## ��{�I�Ȏg����
 `alsl -o filename filename.alsl`
 
 -o�I�v�V�����Ɋg���q�Ȃ��̃t�@�C�������w�肷��ƁAHLSL������.fx�t�@�C����GLSL������.fs�t�@�C�����o�͂���܂�

## �R�}���h���C������

## �ϊ������^

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

## �ϊ������֐�
 + mul(m1, m2): �s��̐�
 + GetTextureSize(texture, sampler)
 + SampleTexture(texture, sampler, uv)

## ���̑��̎d�l
 + �O�����Z�q�͎g�p�s��
 + �ϐ��錾�⃁���o�錾�͈�s�Ɉ��(�^����ɕϐ������J���}�ŋ�؂��ĕ����錾�ł��Ȃ�)
 + `struct{...} var;`�̂悤�ȍ\���̒�`�̒���ɕϐ���錾���邱�Ƃ͂ł��Ȃ�
 + �\���̒�`�̌�̃Z�~�R�����͏ȗ��ł���
 + �萔�錾�ł�const�͌^���̑O�ɂ����L�q�ł��Ȃ�
