CREATE TABLE IF NOT EXISTS MYUSER(
	Nickname TEXT PRIMARY KEY,
	pincode INTEGER(4),
	email TEXT,
	phone TEXT,
	pass TEXT,
);


CREATE TABLE IF NOT EXISTS Fingerprint (
	fingerprintID INTEGER PRIMARY KEY,
	nameFingerprint TEXT,
	Nickname TEXT,
    FOREIGN KEY (Nickname) references MYUSER(Nickname) on update cascade on delete set null
);

CREATE TABLE IF NOT EXISTS Face (
	faceID INTEGER PRIMARY KEY AUTOINCREMENT,
	Nickname TEXT,
	FOREIGN KEY (Nickname) references MYUSER(Nickname) on update cascade on delete set null
);

CREATE TABLE IF NOT EXISTS Image (
	imageID INTEGER PRIMARY KEY AUTOINCREMENT,
	faceID INTEGER NOT NULL,
	facePATH TEXT,
	foreign key (faceID) references Face(faceID) on update cascade on delete cascade
);

CREATE TABLE IF NOT EXISTS MessageInfo(
messageInfoID INTEGER PRIMARY KEY AUTOINCREMENT,
userMessage TEXT,
targetPlatform TEXT,
dateToStart TEXT,
jumpTime INTEGER,
Nickname TEXT,
FOREIGN KEY (Nickname) references MYUSER(Nickname) on update cascade on delete set null


);