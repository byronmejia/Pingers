CREATE TABLE device
(
    id UUID PRIMARY KEY NOT NULL
);
CREATE TABLE ping
(
    time TIMESTAMP NOT NULL,
    device_id UUID NOT NULL,
    CONSTRAINT ping_time_device_id_pk PRIMARY KEY (time, device_id),
    CONSTRAINT ping_device_id_fk FOREIGN KEY (device_id) REFERENCES device (id)
);