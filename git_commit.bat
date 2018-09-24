@cd
@set user_str=
@set x=%date%
@set y=%time:~0,5%
@echo %x% %y%
@cd "C:\Users\TOMOKI\Documents\Aquila-2018master\Aquila-Code-v3"
@git merge
@git add .
@set /P user_str="コミットメッセージを入力してください: "
@git commit -a -m "%x% : %y% : %user_str%"
@git push
@git /P user_str="キーを押してください。終了します。:"