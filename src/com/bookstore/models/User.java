package com.bookstore.models;

import java.io.Serializable;

public abstract class User implements Serializable {
    protected String id;
    protected String name;
    protected String cccd;        
    protected int age;
    protected String address;
    protected String phoneNumber;
    protected String role;
    
    protected User(String id, String name, String cccd, int age, String address, String phoneNumber, String role) {
        this.id = id;
        this.name = name;
        this.cccd = cccd;
        this.age = age;
        this.address = address;
        this.phoneNumber = phoneNumber;
        this.role = role;
    }

    public String getId() { return id; }
    public void setId(String id) { this.id = id; }

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public String getCccd() { return cccd; }
    public void setCccd(String cccd) { this.cccd = cccd; }

    public int getAge() { return age; }
    public void setAge(int age) { this.age = age; }

    public String getAddress() { return address; }
    public void setAddress(String address) { this.address = address; }

    public String getPhoneNumber() { return phoneNumber; }
    public void setPhoneNumber(String phoneNumber) { this.phoneNumber = phoneNumber; }
    
    public String getRole() { return role; }
    
    @Override
    public String toString() {
        return String.format("%s | %s | %s | %d | %s | %s",
                id, name, cccd, age, address, phoneNumber);
    }
}
