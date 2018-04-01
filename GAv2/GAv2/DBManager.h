#pragma once
#include "BackpackProblem.h"

#include <QtCore\qlist.h>
#include <QtSql\qsqldatabase.h>

class DBManager
{
public:
	DBManager(const QString& driverName);

	bool initializeDatabase();
	QVector<BackpackProblem*> *fetchProblems();
	void insertProblem(BackpackProblem *bpp) const;
	void insertSolution(int bppId, int itemCount, SolutionData *data) const;

private:
	QString boolToQString(const bool *data, int size) const;
	bool *qstringToBool(const QString& str) const;

	bool DBFileExists();
	void addDB();
	void createNewDB();

	const QString DB_DRIVER;
	const QString DB_HOST;
	const QString DB_NAME;
	const QString DB_USER;
	const QString DB_PASSWORD;

	mutable QSqlDatabase db;

};