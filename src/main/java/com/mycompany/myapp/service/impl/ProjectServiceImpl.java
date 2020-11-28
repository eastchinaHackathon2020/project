package com.mycompany.myapp.service.impl;

import com.alibaba.fastjson.JSONException;
import com.mycompany.myapp.data.ProjectMapper;
import com.mycompany.myapp.domain.Project;
import com.mycompany.myapp.service.IProjectService;
import com.mycompany.myapp.util.GanttServer;
import com.mycompany.myapp.util.HttpClient4;
import com.mycompany.myapp.vo.FormVO;
import com.mycompany.myapp.vo.ProjectVO;
import com.mycompany.myapp.vo.ResponseVO;

import com.alibaba.fastjson.JSONArray;
import com.mycompany.myapp.vo.TaskVO;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

@Service
public class ProjectServiceImpl implements IProjectService {
    @Autowired
    ProjectMapper projectMapper;

    @Override
    public ResponseVO registerProject(FormVO formVO) {

        // 从task中提取出target
        List<String> targets = new ArrayList<>();
        for (TaskVO taskVO : formVO.getTask()) {
            for (String temp : taskVO.getSubTask()) {
                targets.add(temp);
            }
        }
        formVO.setTargetMilestone(targets);
        try {
            // 请求，判断是否有效，无效返回failure
            String api = revertToAPIURL(formVO.getProjectUrl());
            String query = "?page=0&per_page=1&access_token=c23bf38ff1be1b9fd73e91a757181314b8e702ce";
            System.out.println(revertToAPIURL(formVO.getProjectUrl()));
            String res = HttpClient4.doGet(api + query);
            JSONArray jsonArray = JSONArray.parseArray(res);
            Project project = new Project();
            project.setUserId(formVO.getUserId());
            project.setSha(jsonArray.getJSONObject(0).getString("sha"));
            project.setRepoUrl(formVO.getProjectUrl());
            project.setApiUrl(api);
            String t = "";
            for (String temp : formVO.getTargetMilestone()) {
                t += temp + ';';
            }
            project.setTargetMilestone(t);
            String imageUrl = GanttServer.getImageUrl(api,t);
            project.setImageUrl(imageUrl);
            projectMapper.addProject(project);
            for(TaskVO taskVO:formVO.getTask()){
                String subTask = "";
                for (String temp : taskVO.getSubTask()) {
                    subTask+=temp+';';
                }
                taskVO.setSubTaskString(subTask);
                taskVO.setProjectId(project.getId());
                projectMapper.addTask(taskVO);
            }
            return ResponseVO.buildSuccess(project);
        } catch (JSONException exception) {
            exception.printStackTrace();
            return ResponseVO.buildFailure("Invalid Project's Repository URL. Repo must be public.");
        } catch (Exception e) {
            e.printStackTrace();
            return ResponseVO.buildSuccess("Url ERROR.");
        }
    }

    @Override
    public ResponseVO userProject(String userId) {
        List<Project> projects = projectMapper.getProjectByUserId(userId);
        List<ProjectVO> res = new ArrayList<>();
        for(Project project:projects){
            ProjectVO projectVO = new ProjectVO();
            res.add(projectVO);
            BeanUtils.copyProperties(project,projectVO);
            System.out.println(projectVO);
            List<TaskVO> taskVOS = projectMapper.getTaskByProjectId(project.getId()+"");
            projectVO.setTasks(taskVOS);
            for(TaskVO taskVO:taskVOS){
                System.out.println(taskVO);
                String[] subTasks = taskVO.getSubTaskString().split(";");
                taskVO.setSubTask(new ArrayList<>());
                for(String subTask:subTasks){
                    taskVO.getSubTask().add(subTask);
                }
            }
        }
        return ResponseVO.buildSuccess(res);
    }

    private String revertToAPIURL(String projectUrl) {
        String res = projectUrl.replace("github.com/", "api.github.com/repos/");
        if (res.endsWith("/")) {
            res += "commits";
        } else {
            res += "/commits";
        }
        return res;
    }
}
