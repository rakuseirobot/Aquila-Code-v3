@cd
@set user_str=
@set x=%date%
@set y=%time:~0,5%
@echo %x% %y%
@cd "C:\Users\TOMOKI\Documents\Aquila-2018master\Aquila-Code-v3"
@git merge
@git add .
@set /P user_str="�R�~�b�g���b�Z�[�W����͂��Ă�������: "
@git commit -a -m "%x% : %y% : %user_str%"
@git push
@git /P user_str="�L�[�������Ă��������B�I�����܂��B:"