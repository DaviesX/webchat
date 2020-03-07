SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

ALTER DATABASE demoweb OWNER TO postgres;

CREATE EXTENSION IF NOT EXISTS btree_gin;
CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;
COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';

SET search_path = public, pg_catalog;
SET default_tablespace = '';
SET default_with_oids = false;


CREATE SEQUENCE IF NOT EXISTS auser_group_id_seq
    START WITH 32
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS auser_group (
    id INT NOT NULL DEFAULT nextval('auser_group_id_seq'),
    description VARCHAR(256) UNIQUE NOT NULL,
    permissions BIGINT[] NOT NULL DEFAULT '{}',
    PRIMARY KEY (id)
);

CREATE INDEX IF NOT EXISTS idx_auser_group_description ON auser_group USING btree (description);


CREATE SEQUENCE IF NOT EXISTS auser_id_seq
    START WITH 1024
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS auser (
    id BIGINT NOT NULL DEFAULT nextval('auser_id_seq'),
    user_name VARCHAR(256) UNIQUE NOT NULL,
    alias VARCHAR(256) NOT NULL,
    passcode VARCHAR(1024) NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    avatar_path BIGINT,
    status INT NOT NULL DEFAULT 0,
    group_id INT NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (group_id) REFERENCES auser_group (id) ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_auser_user_name ON auser USING btree (user_name);


CREATE TABLE IF NOT EXISTS friend_request (
    sender_id BIGINT NOT NULL,
    receiver_id BIGINT NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (sender_id, receiver_id),
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (receiver_id) REFERENCES auser (id) ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_friend_request_created_at ON friend_request USING btree (created_at);


CREATE TABLE IF NOT EXISTS friend (
    sender_id BIGINT NOT NULL,
    receiver_id BIGINT NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (sender_id, receiver_id),
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (receiver_id) REFERENCES auser (id) ON DELETE CASCADE
);


CREATE SEQUENCE IF NOT EXISTS message_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS message (
    id BIGINT NOT NULL DEFAULT nextval('message_id_seq'),
    sender_id BIGINT NOT NULL,
    msg VARCHAR(2048),
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE
);


CREATE SEQUENCE IF NOT EXISTS message_queue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS message_queue (
    id BIGINT NOT NULL DEFAULT nextval('message_queue_id_seq'),
    message_id BIGINT NOT NULL,
    receiver_id BIGINT NOT NULL,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (id),
    FOREIGN KEY (message_id) REFERENCES message (id) ON DELETE CASCADE,
    FOREIGN KEY (receiver_id) REFERENCES auser (id) ON DELETE CASCADE
);