package com.mycompany.myapp.domain;


import javax.persistence.*;

@Entity
@Table(name = "record")
public class Record {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;

    @Column(name="user_id",nullable = true)
    private int userId;

    @Column(name="warning",nullable = true)
    private String warning;

    @Column(name="filename",nullable = true)
    private String filename;

    @Column(name="content",nullable = true)
    private String content;

    @Column(name="time",nullable =true)
    private String time;

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public Record(int userId, String warning, String filename,String content,String time) {
        this.userId = userId;
        this.warning = warning;
        this.filename = filename;
        this.content=content;
        this.time=time;
    }

    public Record(){

    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getWarning() {
        return warning;
    }

    public void setWarning(String warning) {
        this.warning = warning;
    }

    public String getFilename() {
        return filename;
    }

    public void setFilename(String filename) {
        this.filename = filename;
    }
}
