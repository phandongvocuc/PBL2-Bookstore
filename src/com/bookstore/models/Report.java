package com.bookstore.models;

import java.io.Serializable;
import java.time.LocalDate;

public class Report implements Serializable {
	private String reportID;
    private String month;              
    private int totalBooksBorrowed;
    private int totalLateReturns;
    private double totalFinesCollected;
    private double totalRevenue;       
    private String topBorrowedBook;    
    private String topReaderName;      
    private String topStaffName;       
    private LocalDate generatedDate;

    public Report() {}

    public Report(String reportID, String month, int totalBooksBorrowed, int totalLateReturns,
            double totalFinesCollected, double totalRevenue,
            String topBorrowedBook, String topReaderName, String topStaffName,
            LocalDate generatedDate) {
	  this.reportID = reportID;
	  this.month = month;
	  this.totalBooksBorrowed = totalBooksBorrowed;
	  this.totalLateReturns = totalLateReturns;
	  this.totalFinesCollected = totalFinesCollected;
	  this.totalRevenue = totalRevenue;
	  this.topBorrowedBook = topBorrowedBook;
	  this.topReaderName = topReaderName;
	  this.topStaffName = topStaffName;
	  this.generatedDate = generatedDate;
    }
    
    public String getReportID() { return reportID; }
    public String getMonth() { return month; }
    public int getTotalBooksBorrowed() { return totalBooksBorrowed; }
    public int getTotalLateReturns() { return totalLateReturns; }
    public double getTotalFinesCollected() { return totalFinesCollected; }
    public double getTotalRevenue() { return totalRevenue; }
    public String getTopBorrowedBook() { return topBorrowedBook; }
    public String getTopReaderName() { return topReaderName; }
    public String getTopStaffName() { return topStaffName; }
    public LocalDate getGeneratedDate() { return generatedDate; }

    public void setReportID(String reportID) { this.reportID = reportID; }
    public void setMonth(String month) { this.month = month; }
    public void setTotalBooksBorrowed(int totalBooksBorrowed) { this.totalBooksBorrowed = totalBooksBorrowed; }
    public void setTotalLateReturns(int totalLateReturns) { this.totalLateReturns = totalLateReturns; }
    public void setTotalFinesCollected(double totalFinesCollected) { this.totalFinesCollected = totalFinesCollected; }
    public void setTotalRevenue(double totalRevenue) { this.totalRevenue = totalRevenue; }
    public void setTopBorrowedBook(String topBorrowedBook) { this.topBorrowedBook = topBorrowedBook; }
    public void setTopReaderName(String topReaderName) { this.topReaderName = topReaderName; }
    public void setTopStaffName(String topStaffName) { this.topStaffName = topStaffName; }
    public void setGeneratedDate(LocalDate generatedDate) { this.generatedDate = generatedDate; }

    public String toString() {
        return reportID + "," + month + "," + totalBooksBorrowed + "," + totalLateReturns + ","
                + totalFinesCollected + "," + totalRevenue + "," + topBorrowedBook + ","
                + topReaderName + "," + topStaffName + "," + generatedDate;
    }

}
