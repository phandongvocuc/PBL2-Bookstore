package com.bookstore.models;

import java.io.Serializable;

public class Staff extends Reader implements Serializable {
    private double dailyFineRate = 0.05; 

    public Staff(String id, String name, String cccd, int age, String address, String phoneNumber) {
        super(id, name, cccd, age, address, phoneNumber);
        this.role = "STAFF";
    }

    public double getDailyFineRate() { return dailyFineRate; }
    public void setDailyFineRate(double dailyFineRate) { this.dailyFineRate = dailyFineRate; }
    
    public boolean canManageUsers() {
    	return true;
    }
}
