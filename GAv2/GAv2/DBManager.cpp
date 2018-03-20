#include "DBManager.h"

#include <QtSql\qsqlquery.h>
#include <QtCore\qcoreapplication.h>
#include <QtCore\qvariant.h>
#include <filesystem>

DBManager::DBManager(const QString& driverName) : DB_DRIVER(driverName), DB_HOST("localhost"), DB_NAME("ga_db"), DB_USER("default_user"), DB_PASSWORD("default_password") { }

bool DBManager::initializeDatabase()
{
	db.setHostName(DB_HOST);
	db.setUserName(DB_USER);
	db.setPassword(DB_PASSWORD);

	if (!DBFileExists())
	{
		addDB();
		if (!db.isValid())
			return false;
		if (!db.open())
			return false;
		createNewDB();
	}
	else
	{
		addDB();
		if (!db.isValid())
			return false;
		if (!db.open())
			return false;
	}
	return true;
}

QVector<BackpackProblem*> *DBManager::fetchProblems()
{
	QVector<BackpackProblem*> *result = new QVector<BackpackProblem*>;
	QSqlQuery query("SELECT * FROM Problems ORDER BY Name ASC");
	query.exec();
	while (query.next())
	{
		//Select items from each problem
		QVector<Item> items;
		QSqlQuery itemQuery("SELECT * FROM Items WHERE IdProblem=?");
		itemQuery.bindValue(0, query.value("IdProblem"));
		itemQuery.exec();
		while (itemQuery.next())
		{
			items.append(Item(itemQuery.value("Weight").toInt(), itemQuery.value("Value").toInt()));
		}

		//Finally create new problem object
		result->append(new BackpackProblem(query.value("IdProblem").toInt(), query.value("Name").toString(), query.value("Capacity").toInt(), move(items)));
	}

	return result;
}

void DBManager::insertProblem(BackpackProblem *bpp) const
{
	db.transaction();
	QSqlQuery query("INSERT INTO Problems (Name, Capacity) VALUES (?, ?)");
	query.bindValue(0, bpp->getName());
	query.bindValue(1, bpp->getBackpackCapacity());
	query.exec();
	QSqlQuery idQuery("SELECT last_insert_rowid()");
	idQuery.exec();
	if (idQuery.next())
		bpp->setId(idQuery.value(0).toInt());

	for (const Item& item : bpp->getItems())
	{
		QSqlQuery itemQuery("INSERT INTO Items (Value, Weight, IdProblem) VALUES (?, ?, ?)");
		itemQuery.bindValue(0, item.getValue());
		itemQuery.bindValue(1, item.getWeight());
		itemQuery.bindValue(2, bpp->getId());
		itemQuery.exec();
	}
	db.commit();
}

void DBManager::insertSolution(int bppId, SolutionData *data) const
{
	db.transaction();
	QSqlQuery query("INSERT INTO Solutions (Name, Capacity) VALUES (?, ?)");


	db.commit();
}

bool DBManager::DBFileExists()
{
	QString path = QCoreApplication::applicationDirPath() + "/" + DB_NAME + ".db";
	return std::experimental::filesystem::is_regular_file(path.toStdString());
}

void DBManager::addDB()
{
	QString path = QCoreApplication::applicationDirPath() + "/" + DB_NAME + ".db";
	db = QSqlDatabase::addDatabase(DB_DRIVER);
	db.setDatabaseName(path);
}

void DBManager::createNewDB()
{
	db.exec("CREATE TABLE Items(IdItem INTEGER NOT NULL PRIMARY KEY, Value integer(10) NOT NULL, Weight integer(10) NOT NULL, IdProblem integer(10) NOT NULL, FOREIGN KEY(IdProblem) REFERENCES Problems(IdProblem));");
	db.exec("CREATE TABLE Problems(IdProblem INTEGER NOT NULL PRIMARY KEY, Name varchar(20) DEFAULT 'defaultName' NOT NULL, Capacity integer(10) NOT NULL);");
	db.exec("CREATE TABLE SelectionParams(IdParam INTEGER NOT NULL PRIMARY KEY, Name text NOT NULL, Value text NOT NULL, IdSolve integer(10) NOT NULL, FOREIGN KEY(IdSolve) REFERENCES Solves(IdSolve));");
	db.exec("CREATE TABLE Solutions(IdSolve INTEGER NOT NULL PRIMARY KEY, MutationRate real(10) NOT NULL, CrossoverRate real(10) NOT NULL, CrossoverPoints integer(10) NOT NULL, PopulationSize integer(10) NOT NULL, GenerationCount integer(10) NOT NULL, SelectionName varchar(255) NOT NULL, Repetitions integer(10) NOT NULL, IdProblem integer(10) NOT NULL, FOREIGN KEY(IdProblem) REFERENCES Problems(IdProblem));");
	db.exec("CREATE TABLE TimePoints(IdSolve integer(10) NOT NULL, GenerationNumber integer(10) NOT NULL, BestValue real(10) NOT NULL, BestIndividual text NOT NULL, AvgValue real(10) NOT NULL, WorstValue real(10) NOT NULL, WorstIndividual text NOT NULL, StdDeviation real(10) NOT NULL, PRIMARY KEY(IdSolve, GenerationNumber), FOREIGN KEY(IdSolve) REFERENCES Solves(IdSolve));");
}
