package com.mycompany.myapp.vo;

import java.util.List;

public class FormVO {
    Long userId;      //用户id
    String projectUrl;  // 仓库地址
    List<TaskVO> task;
    List<String> targetMilestone; // 里程碑

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

    public String getProjectUrl() {
        return projectUrl;
    }

    public void setProjectUrl(String projectUrl) {
        this.projectUrl = projectUrl;
    }

    public List<String> getTargetMilestone() {
        return targetMilestone;
    }

    public void setTargetMilestone(List<String> targetMilestone) {
        this.targetMilestone = targetMilestone;
    }

    public List<TaskVO> getTask() {
        return task;
    }

    public void setTask(List<TaskVO> task) {
        this.task = task;
    }
}
