#���j

 - GLSL��HLSL�̋��ʕ����W������{�ɍ쐬����B
 - HLSL�̃I�u�W�F�N�g�w���I�ȏ�������GLSL�ōČ�������̂ŏȂ�
#���
 - expression: ��
 - statement: ��
 - syntax: �\��
 - semantics: �Z�}���e�B�N�X
 
#�ϐ�

##�錾
`[�C���q] �^�� �ϐ��� [: �Z�}���e�B�N�X] [= ��������];`

##�^��
###�X�J���^
 - `bool`
 - `int`
 - `float`
 - `uint`
 - `double`

###�x�N�g���^
 - (�X�J���^��)(2-4)

### �s��^��
 - .`mat(2-4)`

### swizzle���Z
 - `(�ϐ���)./[xyzw]{1,4}/`
 - `(�ϐ���)./[rgba]{1,4}/`

### �C���q
 - `const`
 - `static`
 - `uniform`


## �\����
`struct �^��{+(�����o�錾)}`

 - �^�����A�������������q���X�g�ւ̑Ή��͗v����
 - �\���̂̓���q�△���\���͕̂s�\
 - �^�[�Q�b�g����R���̕s�\�ȏC�����͌ʑΉ�

## �\��
### if
`if((������)) ��[else ��]`

### for
`for(��������);������;�ď�������) ��`
 - break, continue�̉��߂̓^�[�Q�b�g����C��? �������炢�͌��Ă���������
### while
`while(������) ��`

### do-while
`do �� while(������);`

## ����
`[[������]]`
 - 