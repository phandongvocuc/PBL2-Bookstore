package com.bookstore.models;

import java.io.Serializable;

public class Administrator extends Staff implements Serializable {

    public Administrator(String id, String name, String cccd, int age, String address, String phoneNumber) {
        super(id, name, cccd, age, address, phoneNumber);
        this.role = "ADMIN";
    }

}
