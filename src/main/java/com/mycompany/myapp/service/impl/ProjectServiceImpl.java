package com.mycompany.myapp.service.impl;

import com.alibaba.fastjson.JSONException;
import com.mycompany.myapp.data.ProjectMapper;
import com.mycompany.myapp.domain.Project;
import com.mycompany.myapp.service.IProjectService;
import com.mycompany.myapp.util.HttpClient4;
import com.mycompany.myapp.vo.FormVO;
import com.mycompany.myapp.vo.ResponseVO;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class ProjectServiceImpl implements IProjectService {
    @Autowired
    ProjectMapper projectMapper;

    @Override
    public ResponseVO registerProject(FormVO formVO) {
        try{
            String api = revertToAPIURL(formVO.getProjectUrl());
            System.out.println(revertToAPIURL(formVO.getProjectUrl()));
            String res = HttpClient4.doGet(api);
            JSONArray jsonArray = JSONArray.parseArray(res);
            Project project = new Project();
            project.setUserId(formVO.getUserId());
            project.setCommitNum(jsonArray.size());
            project.setRepoUrl(formVO.getProjectUrl());
            project.setApiUrl(api);
            //TODO:modify image url
            project.setImageUrl("testURL");
            String targets = "";
            for(String temp:formVO.getTargetMilestone()){
                targets+=temp+';';
            }
            project.setTargetMilestone(targets);
            projectMapper.addProject(project);
            return ResponseVO.buildSuccess(project);
        }catch (JSONException exception){
            exception.printStackTrace();
            return ResponseVO.buildFailure("Invalid Project's Repository URL. Repo must be public.");
        }catch (Exception e){
            e.printStackTrace();
            return ResponseVO.buildSuccess("Url ERROR.");
        }
    }

    private String revertToAPIURL(String projectUrl){
        String res  = projectUrl.replace("github.com/","api.github.com/repos/");
        if (res.endsWith("/")){
            res+="commits";
        }else{
            res+="/commits";
        }
        return res;
    }
}
