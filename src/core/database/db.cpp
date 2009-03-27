#include "db.h"

DataBase::DataBase(){
   return;
}

bool DataBase::checkDb(QStringList tables){
   /*
    * Checking database tables.
    * If not exists, we try to create them.
    */

   QSqlDatabase db = QSqlDatabase::database();
   QSqlQuery query;
   QString table="";

   for (int i=0; i<tables.size(); ++i){
	table=tables.at(i).toLocal8Bit().constData();

	if (db.record(table).isEmpty()){
	   if (table == "prefix"){
		if(!query.exec("CREATE TABLE prefix (wine_dllpath TEXT, wine_loader TEXT, wine_server TEXT, wine_exec TEXT, cdrom_mount TEXT, cdrom_drive TEXT, id INTEGER PRIMARY KEY, name TEXT, path TEXT, version TEXT);"))
		   return FALSE;

		   //Creating default prefix reccord
		   query.prepare("INSERT INTO prefix(id, name) VALUES(NULL, :name);");
		   query.bindValue(":name", "Default");
		   if (!query.exec())
			return FALSE;
		   query.clear();
	   }
	   if (table == "dir"){
		if(!query.exec("CREATE TABLE dir (id INTEGER PRIMARY KEY, name TEXT, prefix_id NUMERIC);"))
		   return FALSE;
	   }
	   if (table == "icon"){
		if(!query.exec("CREATE TABLE icon (wrkdir TEXT, override TEXT, winedebug TEXT, useconsole NUMERIC, display TEXT, cmdargs TEXT, exec TEXT, icon_path TEXT, desc TEXT, desktop TEXT, nice TEXT, dir_id NUMERIC, id INTEGER PRIMARY KEY, name TEXT, prefix_id NUMERIC);"))
		   return FALSE;
	   }
	   if (table == "images"){
		if(!query.exec("CREATE TABLE images (id INTEGER PRIMARY KEY, name TEXT, path TEXT);"))
		   return FALSE;
	   }
	}
   }

   return TRUE;
}

void DataBase::close(){
   QSqlDatabase db = QSqlDatabase::database();
   db.close();
   return;
}
