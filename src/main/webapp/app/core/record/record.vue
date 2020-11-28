<template>
    <div id="record" style="background-color: white">
        <div class="jumbotron masthead">
            <div class="container">
                <h1>XAnalysis</h1>
                <h2>您的代码检测记录如下</h2>
            </div>
        </div>

        <b-list-group flush style="margin-top: 30px">
            <b-list-group-item>
                <div style="display: flex;justify-content:space-between;">
                    <h1 style="font-size: 23px;font-weight: bold;width: 400px;text-align: left">项目地址</h1>
<!--                    <h1 style="font-size: 23px;font-weight: bold;width: 400px;text-align: left">报错数</h1>-->
                    <h1 style="font-size: 23px;font-weight: bold;text-align: right">图片路径</h1>
                </div>
            </b-list-group-item>
            <b-list-group-item v-for="project in this.allProjects" >
                <div style="display: flex;justify-content:space-between;"  @click="jumpToDetail(project)">
                    <h1 style="font-size: 23px;font-weight: normal;width: 400px;text-align: left">{{project.githubAddress}}</h1>
<!--                    <h1 style="font-size: 23px;font-weight: normal;width: 400px;text-align: left">{{res.warning.split('\n').length-1}}</h1>-->
                    <h1 style="font-size: 23px;font-weight: normal;text-align: right">{{project.imgUrl}}</h1>
                </div>
            </b-list-group-item>
        </b-list-group>

    </div>
</template>

<script>

    import axios from 'axios';

    export default {
        name: "Record",
        data(){
            return{
                allProjects:[
                 {
                     imgUrl:"https://www.jb51.net/article/185112.htm",
                     githubAddress:"asdfsaddsasdagdsagsdagdsg",
                     tasks:
                     [
                        {
                            name: "task1",
                            subTask: ["subtask1", "subtask2", "subtask3"]
                        },
                        {
                            name: "task2",
                            subTask: ["subtask4", "subtask5", "subtask6"],

                        },
                        {
                            name: "task3",
                            subTask: ["subtask6", "subtask1", "subtask8"],
                        }
                    ]
                },
                {
                    imgUrl:"https://www.jbdfa51.net/article/185112.htm",
                    githubAddress:"asdfsaddsasdadsfasfagdsagsdagdsg",
                    tasks:
                        [
                            {
                                name: "task9",
                                subTask: ["subtask1", "subtask2", "subtask3"]
                            },
                            {
                                name: "task31",
                                subTask: ["subtask4", "subtask5", "subtask6"],

                            },
                            {
                                name: "task34",
                                subTask: ["subtask6", "subtask1", "subtask8"],
                            }
                        ]
                },
                {
                    imgUrl:"https://www.jb51.net/article/185112.htm",
                    githubAddress:"asdfsaddsasdagdsagsdagdsg",
                    tasks:
                        [
                            {
                                name: "task10",
                                subTask: ["subtask1", "subtask2", "subtask3"]
                            },
                            {
                                name: "task18",
                                subTask: ["subtask4", "subtask5", "subtask6"],

                            },
                            {
                                name: "task123",
                                subTask: ["subtask6", "subtask1", "subtask8"],
                            }
                        ]
                }

                ],


                //
                numList:[],
                recordList:[],
            }
        },
        async mounted() {
            console.log(this.$store.getters.account.id)
            axios
                .get('api/'+this.$store.getters.account.id+'/getUserRecord')
                .then(result => {
                    console.log(result)
                    this.myResList=result.data;
                    console.log(this.myResList)
                });
        },
        methods:{
            jumpToDetail(project){
                this.$store.commit('setProject',project)
                //将当前的project commit到store
                this.$router.push({name: 'Result'})
            }
        }
    }
</script>

<style scoped>
    #record{
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
        padding: 35px 30px 20px 30px;
    }

    .masthead h1 {
        font-size: 70px;
        font-weight:bold;
        margin-bottom: 20px;
        color:#ffffff;
    }

    .masthead h2 {
        color:#ffffff;
        font-weight:normal;
        margin-bottom:15px;
    }

    .masthead {
        margin-bottom: 30px;
        color: #fff;
    }

    .jumbotron {
        position: relative;
        padding: 25px 25px;
        color: #fff;
        text-align: left;
        text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
        border-radius: 0;
        background: #020031;
        background: linear-gradient(45deg,#43d8c9,#035aa6);
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
        box-shadow: inset 0 3px 7px rgba(255,255,255,0), inset 0 -3px 7px rgba(255,255,255,0);
    }
</style>
