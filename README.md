# Visual Gitwork

> 通过持续监控Github的commit信息，生成并自动化更新体现项目进度Gantt chart

## 需求场景

- 疫情期间以及当今疫情笼罩的背景下，教学和项目团队产生监督困难，分布式管理工具在协作开发上提供便利但在进度监督以及项目管理方面缺失严重。导致管理人员对项目进度，以及团队成员工作情况没有清晰的认知，无法精确地了解目前教学进度/项目开发程度，从而造成在疫情隔离期间使用Github进行协同开发（项目或作业）效率低下。

## 应用场景

* 课程**编译原理**教师使用的仓库https://github.com/courses-at-nju-by-hfwei/compilers-lectures进行应用

	![](https://gitvisual.oss-cn-beijing.aliyuncs.com/compilers-lectures.png)

* 自制项目https://github.com/eastchinaHackathon2020/testcase

	![](https://gitvisual.oss-cn-beijing.aliyuncs.com/testcase.png)

## 项目功能

- 官方网站输入项目地址，提供用户一个URL图床地址，仅需在Readme中添加链接，工具即可通过持续监控Github项目成员的commit信息，实时更新项目甘特图，不同成员通过不同颜色区分，图片通过提供用户的唯一url进行readme的更新。
- 另外，用户也可以在我们的网站上登陆，只需输入github项目地址，即可监控当前项目进度/教学进度；用户也可以在网站中浏览自己所有添加监视的仓库，一一跟进项目进度。
- ![](https://spring-security.oss-cn-beijing.aliyuncs.com/img/image-20201129115135480.png)
- ![](https://spring-security.oss-cn-beijing.aliyuncs.com/img/image-20201129115206549.png)
- ![](https://spring-security.oss-cn-beijing.aliyuncs.com/img/image-20201129115232737.png)

