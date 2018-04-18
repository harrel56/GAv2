#include "DBManager.h"
#include "SolutionData.h"

#include <QtSql\qsqlquery.h>
#include <QtCore\qcoreapplication.h>
#include <QtCore\qvariant.h>
#include <filesystem>
#include <iostream>

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
	db.transaction();
	QVector<BackpackProblem*> *result = new QVector<BackpackProblem*>;
	QSqlQuery query("SELECT * FROM Problems ORDER BY Name ASC");
	query.exec();
	while (query.next())
	{
		//Select items from each problem
		QVector<Item> items;
		QSqlQuery itemQuery("SELECT * FROM Items WHERE IdProblem=? ORDER BY IdItem");
		itemQuery.bindValue(0, query.value("IdProblem"));
		itemQuery.exec();
		while (itemQuery.next())
		{
			items.append(Item(itemQuery.value("Weight").toInt(), itemQuery.value("Value").toInt()));
		}

		//Select solutions
		QVector<SolutionData*> solutions;
		QSqlQuery solQuery("SELECT * FROM Solutions WHERE IdProblem=?");
		solQuery.bindValue(0, query.value("IdProblem"));
		solQuery.exec();
		while (solQuery.next())
		{
			//Select selection params
			QVector<std::pair<QString, QString>> params;
			QSqlQuery paramQuery("SELECT * FROM SelectionParams WHERE IdSolution=?");
			paramQuery.bindValue(0, solQuery.value("IdSolution"));
			paramQuery.exec();
			while (paramQuery.next())
			{
				params.append(std::make_pair(paramQuery.value("Name").toString(), paramQuery.value("Value").toString()));
			}

			//Select time points
			QVector<TimePoint> *timePoints = new QVector<TimePoint>(solQuery.value("GenerationCount").toInt());
			QSqlQuery pointsQuery("SELECT * FROM TimePoints WHERE IdSolution=?");
			pointsQuery.bindValue(0, solQuery.value("IdSolution"));
			pointsQuery.exec();
			while (pointsQuery.next())
			{
				(*timePoints)[pointsQuery.value("GenerationNumber").toInt() - 1] = TimePoint(pointsQuery.value("BestValue").toInt(), pointsQuery.value("AvgValue").toDouble(), pointsQuery.value("WorstValue").toInt(), pointsQuery.value("StdDeviation").toDouble(),
					qstringToBool(pointsQuery.value("BestIndividual").toString()), qstringToBool(pointsQuery.value("WorstIndividual").toString()));
			}

			solutions.append(new SolutionData(solQuery.value("Name").toString(), solQuery.value("SelectionName").toString(), move(params), timePoints,
				solQuery.value("PopulationSize").toInt(), solQuery.value("GenerationCount").toInt(), solQuery.value("MutationRate").toDouble(), solQuery.value("CrossoverRate").toDouble(),
				solQuery.value("CrossoverPoints").toInt(), solQuery.value("Repetitions").toInt(), solQuery.value("PenaltyFixed").toDouble(), solQuery.value("PenaltyParam").toDouble(), solQuery.value("PenaltyPower").toDouble()));
		}

		//Finally create new problem object
		result->append(new BackpackProblem(query.value("IdProblem").toInt(), query.value("Name").toString(), query.value("Capacity").toInt(), move(items), move(solutions)));
	}
	db.commit();
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

void DBManager::insertSolution(int bppId, int itemCount, SolutionData *data) const
{
	db.transaction();
	QSqlQuery query("INSERT INTO Solutions (Name, MutationRate, CrossoverRate, CrossoverPoints, PopulationSize, GenerationCount, SelectionName, Repetitions, PenaltyFixed, PenaltyParam, PenaltyPower, IdProblem) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	query.bindValue(0, data->getName());
	query.bindValue(1, data->getMutationChance());
	query.bindValue(2, data->getCrossChance());
	query.bindValue(3, data->getCrossPoints());
	query.bindValue(4, data->getPopSize());
	query.bindValue(5, data->getGenerationCount());
	query.bindValue(6, data->getSelectionName());
	query.bindValue(7, data->getRepetitions());
	query.bindValue(8, data->getPenaltyFixed());
	query.bindValue(9, data->getPenaltyParam());
	query.bindValue(10, data->getPenaltyPower());
	query.bindValue(11, bppId);
	query.exec();

	QSqlQuery idQuery("SELECT last_insert_rowid()");
	idQuery.exec();
	int solId = -1;
	if (idQuery.next())
		solId = idQuery.value(0).toInt();

	for (auto param : data->getSelectionParams())
	{
		QSqlQuery paramQuery("INSERT INTO SelectionParams (Name, Value, IdSolution) VALUES (?, ?, ?)");
		paramQuery.bindValue(0, param.first);
		paramQuery.bindValue(1, param.second);
		paramQuery.bindValue(2, solId);
		paramQuery.exec();
	}

	int genCounter = 1;
	for (const auto& point : *data->getTimePoints())
	{
		QSqlQuery pointQuery("INSERT INTO TimePoints (IdSolution, GenerationNumber, BestValue, BestIndividual, AvgValue, WorstValue, WorstIndividual, StdDeviation) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
		pointQuery.bindValue(0, solId);
		pointQuery.bindValue(1, genCounter);
		pointQuery.bindValue(2, point.bestValue);
		pointQuery.bindValue(3, boolToQString(point.bestData, itemCount));
		pointQuery.bindValue(4, point.avgValue);
		pointQuery.bindValue(5, point.worstValue);
		pointQuery.bindValue(6, boolToQString(point.worstData, itemCount));
		pointQuery.bindValue(7, point.deviationValue);
		pointQuery.exec();
		++genCounter;
	}

	db.commit();
}

QString DBManager::boolToQString(const bool *data, int size) const
{
	QString result(size);
	for (int i = 0; i < size; ++i)
	{
		result[i] = data[i] ? '1' : '0';
	}

	return result;
}

bool * DBManager::qstringToBool(const QString & str) const
{
	bool *result = new bool[str.size()];
	for (int i = 0; i < str.size(); ++i)
	{
		result[i] = str.at(i) == '1' ? true : false;
	}
	return result;
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
	db.transaction();
	db.exec("CREATE TABLE Items(IdItem INTEGER NOT NULL PRIMARY KEY, Value integer(10) NOT NULL, Weight integer(10) NOT NULL, IdProblem integer(10) NOT NULL, FOREIGN KEY(IdProblem) REFERENCES Problems(IdProblem));");
	db.exec("CREATE TABLE Problems(IdProblem INTEGER NOT NULL PRIMARY KEY, Name varchar(20) DEFAULT 'defaultName' NOT NULL, Capacity integer(10) NOT NULL);");
	db.exec("CREATE TABLE SelectionParams(IdParam INTEGER NOT NULL PRIMARY KEY, Name text NOT NULL, Value text NOT NULL, IdSolution integer(10) NOT NULL, FOREIGN KEY(IdSolution) REFERENCES Solutions(IdSolution));");
	db.exec("CREATE TABLE Solutions(IdSolution INTEGER NOT NULL PRIMARY KEY, Name text NOT NULL, MutationRate real(10) NOT NULL, CrossoverRate real(10) NOT NULL, CrossoverPoints integer(10) NOT NULL, PopulationSize integer(10) NOT NULL, GenerationCount integer(10) NOT NULL, SelectionName varchar(255) NOT NULL, Repetitions integer(10) NOT NULL, PenaltyFixed real(10) NOT NULL, PenaltyParam real(10) NOT NULL, PenaltyPower real(10) NOT NULL, IdProblem integer(10) NOT NULL, FOREIGN KEY(IdProblem) REFERENCES Problems(IdProblem));");
	db.exec("CREATE TABLE TimePoints(IdSolution integer(10) NOT NULL, GenerationNumber integer(10) NOT NULL, BestValue real(10) NOT NULL, BestIndividual text NOT NULL, AvgValue real(10) NOT NULL, WorstValue real(10) NOT NULL, WorstIndividual text NOT NULL, StdDeviation real(10) NOT NULL, PRIMARY KEY(IdSolution, GenerationNumber), FOREIGN KEY(IdSolution) REFERENCES Solutions(IdSolution));");
	db.commit();
}
