rm stop
nohup sudo ./clim8 -log Cave Dehors Radiateur &

history()
{
	WWW=/var/www/html
	ln index.html $WWW/index.html
	ln current.csv $WWW/current.csv

	sudo vi /etc/rc.local

}

