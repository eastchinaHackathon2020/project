package com.mycompany.myapp.vo;

import java.security.SecureRandom;
import java.util.List;

public class TaskVO {
    private Long id;
    private String name;
    private List<String> subTask;
    private String subTaskString;
    private Long projectId;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Long getProjectId() {
        return projectId;
    }

    public void setProjectId(Long projectId) {
        this.projectId = projectId;
    }

    public String getSubTaskString() {
        return subTaskString;
    }

    public void setSubTaskString(String subTaskString) {
        this.subTaskString = subTaskString;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<String> getSubTask() {
        return subTask;
    }

    public void setSubTask(List<String> subTask) {
        this.subTask = subTask;
    }

    @Override
    public String toString() {
        return "TaskVO{" +
            "id=" + id +
            ", name='" + name + '\'' +
            ", subTask=" + subTask +
            ", subTaskString='" + subTaskString + '\'' +
            ", projectId=" + projectId +
            '}';
    }
}
