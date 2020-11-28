package com.mycompany.myapp.controller;

import com.mycompany.myapp.service.IProjectService;
import com.mycompany.myapp.vo.FormVO;
import com.mycompany.myapp.vo.ResponseVO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/project")
public class ProjectController {
    @Autowired
    IProjectService projectService;

    @PostMapping("registerProject")
    public ResponseVO registerProject(@RequestBody FormVO formVO){
        return projectService.registerProject(formVO);
    }
}
