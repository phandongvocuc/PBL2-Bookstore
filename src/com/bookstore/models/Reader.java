package com.bookstore.models;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Reader extends User implements Serializable {
    private List<Rental> rentalHistory;
    private List<Rental> currentRentals;

    public Reader(String id, String name, String cccd, int age, String address, String phoneNumber) {
        super(id, name, cccd, age, address, phoneNumber, "READER");
        this.rentalHistory = new ArrayList<>();
        this.currentRentals = new ArrayList<>();
    }

    public List<Rental> getRentalHistory() { return rentalHistory; }
    public List<Rental> getCurrentRentals() { return currentRentals; }

    public void addRental(Rental rental) {
        currentRentals.add(rental);
    }

    public void returnBook(Rental rental) {
        currentRentals.remove(rental);
        rentalHistory.add(rental);
    }
}
