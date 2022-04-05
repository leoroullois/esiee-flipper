CREATE DATABASE IF NOT EXISTS `IGI-3014-ROULLOIS`;
USE `IGI-3014-ROULLOIS`;
DROP TABLE IF EXISTS `Responsable`;
DROP TABLE IF EXISTS `Epoque`;
DROP TABLE IF EXISTS `Adresse`;
DROP TABLE IF EXISTS `Coordonnees`;
DROP TABLE IF EXISTS `Operation`;
DROP TABLE IF EXISTS `Decouverte`;
DROP TABLE IF EXISTS `Appartenir`;
CREATE TABLE Responsable(
  idResponsable COUNTER,
  nomResponsable VARCHAR(100),
  PRIMARY KEY(idResponsable)
);
CREATE TABLE Epoque(
  idEpoque COUNTER,
  libelleEpoque VARCHAR(50) NOT NULL,
  PRIMARY KEY(idEpoque)
);
CREATE TABLE Adresse(
  idAdresse COUNTER,
  codePostale DECIMAL(5, 0),
  adresse VARCHAR(100),
  commune VARCHAR(100),
  PRIMARY KEY(idAdresse)
);
CREATE TABLE IF NOT EXISTS Coordonnees(
  idCoordonnees COUNTER,
  X DOUBLE NOT NULL,
  Y DOUBLE NOT NULL,
  geoShape TEXT,
  geoPoint2D TEXT,
  PRIMARY KEY(idCoordonnees)
);
CREATE TABLE Operation(
  idOperation COUNTER,
  dateOperation DATE,
  natureOperation VARCHAR(50),
  idResponsable_organise INT NOT NULL,
  PRIMARY KEY(idOperation),
  FOREIGN KEY(idResponsable_organise) REFERENCES Responsable(idResponsable)
);
CREATE TABLE Decouverte(
  idDecouverte SMALLINT,
  synthese VARCHAR(8000),
  idCoordonnees_localise INT,
  idOperation_decouvre INT NOT NULL,
  idAdresse_situe INT,
  PRIMARY KEY(idDecouverte),
  FOREIGN KEY(idCoordonnees_localise) REFERENCES Coordonnees(idCoordonnees),
  FOREIGN KEY(idOperation_decouvre) REFERENCES Operation(idOperation),
  FOREIGN KEY(idAdresse_situe) REFERENCES Adresse(idAdresse)
);
CREATE TABLE Appartenir(
  idEpoque_appartient INT,
  idDecouverte_a_appartenu SMALLINT,
  PRIMARY KEY(idEpoque_appartient, idDecouverte_a_appartenu),
  FOREIGN KEY(idEpoque_appartient) REFERENCES Epoque(idEpoque),
  FOREIGN KEY(idDecouverte_a_appartenu) REFERENCES Decouverte(idDecouverte)
);
-- Suppression des données existantes
USE `IGI-3014-ROULLOIS`;
DROP TABLE IF EXISTS Responsable;
DROP TABLE IF EXISTS Epoque;
DROP TABLE IF EXISTS Adresse;
DROP TABLE IF EXISTS Coordonnees;
DROP TABLE IF EXISTS Operation;
DROP TABLE IF EXISTS Decouverte;
DROP TABLE IF EXISTS Appartenir;

-- Insertion des données
INSERT INTO Responsable(nomResponsable) VALUES('ROULLOIS');