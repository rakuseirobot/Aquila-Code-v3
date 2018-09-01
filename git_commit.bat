@cd
@set x=%date%
@set y=%time:~0,5%
@echo %x% %y%
@cd "C:\Users\TOMOKI\Documents\Aquila-2018master\Aquila-Code-v3"
@git merge
@git add .
@git commit -a -m "%x% : %y%"
@git push