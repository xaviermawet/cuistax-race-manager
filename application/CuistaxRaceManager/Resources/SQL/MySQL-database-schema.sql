-- Drop all tables
DROP TABLE IF EXISTS `lap`;
DROP TABLE IF EXISTS `race`;
DROP TABLE IF EXISTS `team`;

-- Create all tables

create table team
(
    cuistax	INTEGER PRIMARY KEY,		-- Cuistax number
    name	VARCHAR(80)	NOT NULL UNIQUE	-- Team name (unique)
);

create table race
(
    id			INTEGER PRIMARY KEY AUTO_INCREMENT,	-- Race unique id
    name		VARCHAR(80),						-- Race name
    date		DATE,								-- Race date
    location	VARCHAR(256),						-- Race location. Absolute location (street address)
    length		FLOAT								-- Race length in meters
);

create table lap
(
    num		INTEGER,		-- Lap number
    time	TIME NOT NULL,	-- Lap time
    date	DATETIME,		-- Lap datetime

    race	INTEGER,		-- Race reference
    team	INTEGER,		-- Team reference

    FOREIGN KEY (race) REFERENCES race(id) ON DELETE CASCADE,		-- delete the lap if the referenced race is deleted
    FOREIGN KEY (team) REFERENCES team(cuistax) ON DELETE CASCADE,	-- delete the lap if the referenced team is deleted
    PRIMARY KEY (num, team, race)
);
