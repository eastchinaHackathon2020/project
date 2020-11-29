<template>
    <div style="height: 100%">
        <div id="wrapper" v-if="!this.loading" style="background-color: white">
            <div class="jumbotron1 masthead1">
                <h1>Visual Gitwork</h1>
                <h2>输入您需要添加插件的GitHub仓库地址</h2>
                <hr>
                <!-- <h3 style="font-size: 25px;font-weight: normal;margin-top: 30px">请上传您需要检测的CPP代码文件</h3> -->
                <div style="padding-left: 320px;padding-right: 320px;margin-top: 30px">
                    <b-input-group class="mt-3">
                        <b-form-input v-model="form.projectUrl"></b-form-input>
                        <b-input-group-append>
<!--                            <b-button variant="success"  @click="this.$bvModal.show('bv-modal-example')">确认</b-button>-->
                            <b-button variant="success" @click="modalShow = !modalShow">确认</b-button>
                        </b-input-group-append>
                    </b-input-group>




                    <b-modal id="bv-modal-example" hide-footer v-model="modalShow">
                        <template #modal-title>
<!--                        Using <code>$bvModal</code> Methods-->
                            <div>请设置您的项目</div>
                        </template>
                        <div class="d-block "  style="min-height:450px; height:auto" >
                            <div  style="display: flex;justify-content:space-between;align-items: center; ">
                                <div style="color:#3E8ACC;padding-left: 30px" v-model="choosedTaskName" >{{"Now: "+choosedTaskName}}</div>
                                <b-button
                                    variant="link"
                                    size=""
                                    style="color:#28a745;padding-right: 30px"
                                    @click="choosedTaskName='/' "
                                >Back
                                </b-button>
                            </div>

                            <b-input-group  style="margin-top: 0;padding:0 20px 20px 30px;" size="">
                                <b-form-input v-model="addTaskName=value"></b-form-input>
                                <b-input-group-append>
                                    <!--                            <b-button variant="success"  @click="this.$bvModal.show('bv-modal-example')">确认</b-button>-->
                                    <b-button variant="info"  size="" @click="addTask()">Add</b-button>
                                </b-input-group-append>
                            </b-input-group>

                            <div  v-for="taskElem in this.tasks"  style=" margin-left:50px;margin-right: 30px;margin-top: 10px ;display: flex;justify-content:space-around;align-items: center">
                                <b-dropdown
                                    block
                                    split
                                    split-variant="outline-primary"
                                    variant="primary"
                                    :text="taskElem.name"
                                    @click="chooseTask(taskElem)"
                                    size=""
                                    style="width: 270px;margin-right:10px  "
                                >
                                    <div href="#"
                                                     v-for="subTaskElem in taskElem.subTask"
                                                     style="width: 270px; height: 30px;display: flex;justify-content:space-between;align-items: center"
                                    >
                                        <div style="padding-left: 20px">{{ subTaskElem }}</div>
                                        <b-button
                                            variant="link"
                                            size=""
                                            style="padding-right: 20px"
                                            @click="delSubTask(subTaskElem)"
                                        >remove
                                        </b-button>

                                    </div>

    <!--                                    <b-button-->
    <!--                                        @click="removeTag(tag)"-->
    <!--                                        variant="link"-->
    <!--                                        size=""-->
    <!--                                        :aria-controls="`my-custom-tags-tag_${tag.replace(/\s/g, '_')}_`"-->
    <!--                                    >remove</b-button>-->

                                </b-dropdown>
                                <b-button variant="outline-danger" @click="delTask(taskElem)" size="">DEL</b-button>
                            </div>
<!--                        <h3>Hello From This Modal!</h3>-->
                        </div>
<!--                        <b-button class="mt-3" block @click="$bvModal.hide('bv-modal-example')">Close Me</b-button>-->
                        <b-button variant="success" class="mt-3" block @click="submitProject()">确定</b-button>
                    </b-modal>
                </div>

            </div>

        </div>

        <div id="wrapper1" v-if="this.loading">
            <div class="jumbotron masthead">
                <div class="spinner">
                    <div class="spinner-container container1">
                        <div class="circle1"></div>
                        <div class="circle2"></div>
                        <div class="circle3"></div>
                        <div class="circle4"></div>
                    </div>
                    <div class="spinner-container container2">
                        <div class="circle1"></div>
                        <div class="circle2"></div>
                        <div class="circle3"></div>
                        <div class="circle4"></div>
                    </div>
                    <div class="spinner-container container3">
                        <div class="circle1"></div>
                        <div class="circle2"></div>
                        <div class="circle3"></div>
                        <div class="circle4"></div>
                    </div>
                </div>
                <h3 style="margin-top: 50px;font-size: 40px;font-weight: bold;">Loading stuff...</h3>
            </div>
        </div>
    </div>
</template>

<script>
import { merge } from "../api/upload";
import {registerProject} from "../api/project";
import { Store,mapState } from 'vuex';
import axios from 'axios';


// tasks:[
//     {
//         name:"task1",
//         subTask:[
//             {
//                 subname:"subTask1",
//             },
//             {
//                 subname:"subTask4",
//             },
//             {
//                 subname:"subTask7",
//             },
//         ]
//     },
//     {
//         name:"task2",
//
//         subTask:[
//             {
//                 subname:"subTask2",
//             }
//         ]
//     },
//     {
//         name:"task3",
//
//         subTask:[
//             {
//                 subname:"subTask3",
//
//             }
//         ]
//     },
// ],
export default{
    name: "Upload",
    data() {
    return {

        form:{
            userId:-1,
            projectUrl:'',
            task:[],
            targetMilestone:[],
        },
      //!!
        modalShow:false,
        choosedTaskName:"/",
        addTaskName:"",
      //
      formshow:false,
      loading:false,
      panelShow: false, //选择文件后，展示上传panel
      collapse: false,
      files: [],

      // 添加任务阶段并不涉及到 id！！
        tasks:[
        ],
      attrs: {
      },
    }
  },

  methods: {
    submitProject(){
        this.modalShow = !this.modalShow;
        this.form.userId=this.$store.getters.account.id;
        /* for(let i=0;i<this.tasks.length;i++){
            this.tasks[i]
        } */
        this.form.task=this.tasks;
        
        //console.log(this.form.projectUrl);
        axios
        .post('api/registerProject',this.form)
            .then(result => {
                console.log(result);
                this.form.projectUrl='';
        });
        

    },
    showform(){
        this.formshow=true;
    },
    //
      chooseTask(taskElem){
        this.choosedTaskName=taskElem.name;


      },
      addTask(){
          // console.log(this.addTaskName);
          if(this.choosedTaskName!="/"){
          //    find task
              for(let i = 0; i < this.tasks.length; i++) {
                  if(this.tasks[i].name==this.choosedTaskName) {
                      this.tasks[i].subTask.push(this.addTaskName);
                      break;
                  }
              }
          }else {
          //    直接添加到主任务下。
             this.tasks.push({name:this.addTaskName,subTask:[]});
          }
      },
      delTask(taskElem){
          console.log("del");
          console.log(taskElem.name);
          for(let i = 0; i < this.tasks.length; i++) {
              if(this.tasks[i].name==taskElem.name) {
                  this.tasks.splice(i,1);
                  break;
              }
          }
      },
      delSubTask(subTaskElem){
          console.log("delSub");
          console.log(subTaskElem);
          var isDel=0;
          for(let i = 0; i < this.tasks.length; i++) {
              if(this.tasks[i].name==this.choosedTaskName) {
                  for(let j = 0; j < this.tasks[i].subTask.length; j++){
                       console.log("Jinru l");
                      if(this.tasks[i].subTask[j]==subTaskElem){
                             console.log("zhaodaol");
                          this.tasks[i].subTask.splice(j,1);
                          isDel=1;
                          break;
                      }
                  }
                  if(isDel==1){
                      break;
                  }
              }
          }
      },
    //
    /* onFileAdded1(file) {
      console.log(file);
    },
    onFileProgress1(rootFile, file, chunk) {
        this.loading=true
    },
    onFileSuccess1(rootFile, file, response, chunk) {
        this.loading=true
        let res = JSON.parse(response);
        console.log(res)
        this.$store.commit('setPassage',res.content)
        this.$store.commit('setWarning',res.warning)
        this.$router.push({name: 'Result'})
    },
    onFileError1(rootFile, file, response, chunk) {
    } */
  }
}
</script>

<style scoped>
    #wrapper{
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
        padding: 30px;
    }

    #wrapper1{
        width: 100%;
        height: 100%;
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
        padding: 40px 40px 15px;
    }

    .jumbotron1 {
        position: relative;
        padding: 160px 160px;
        color: #fff;
        text-align: center;
        text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
        background: #020031;
        background: linear-gradient(45deg,#43d8c9,#035aa6);
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
        box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
    }

    .jumbotron {
        padding: 215px 215px;
        color: #fff;
        text-align: center;
        text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
        background: #020031;
        background: linear-gradient(45deg,#43d8c9,#035aa6);
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
        box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
    }

    .masthead1 h1 {
        font-size: 110px;
        font-weight:bold;
        margin-bottom: 30px;
        color:#ffffff;
    }

    .masthead1 h2 {
        color:#ffffff;
        font-weight:normal;
        margin-bottom:30px;
    }


    #link{
        color: rgba(0, 0, 0, 0.99);
    }


    .spinner {
        margin: 30px auto;
        width: 130px;
        height: 130px;
        position: relative;
    }

    .container1 > div, .container2 > div, .container3 > div {
        width: 28px;
        height: 28px;
        background: #ffffff;

        border-radius: 100%;
        position: absolute;
        -webkit-animation: bouncedelay 1.2s infinite ease-in-out;
        animation: bouncedelay 1.2s infinite ease-in-out;
        -webkit-animation-fill-mode: both;
        animation-fill-mode: both;
    }

    .spinner .spinner-container {
        position: absolute;
        width: 100%;
        height: 100%;
    }

    .container2 {
        -webkit-transform: rotateZ(45deg);
        transform: rotateZ(45deg);
    }

    .container3 {
        -webkit-transform: rotateZ(90deg);
        transform: rotateZ(90deg);
    }

    .circle1 { top: 0; left: 0; }
    .circle2 { top: 0; right: 0; }
    .circle3 { right: 0; bottom: 0; }
    .circle4 { left: 0; bottom: 0; }

    .container2 .circle1 {
        -webkit-animation-delay: -1.1s;
        animation-delay: -1.1s;
    }

    .container3 .circle1 {
        -webkit-animation-delay: -1.0s;
        animation-delay: -1.0s;
    }

    .container1 .circle2 {
        -webkit-animation-delay: -0.9s;
        animation-delay: -0.9s;
    }

    .container2 .circle2 {
        -webkit-animation-delay: -0.8s;
        animation-delay: -0.8s;
    }

    .container3 .circle2 {
        -webkit-animation-delay: -0.7s;
        animation-delay: -0.7s;
    }

    .container1 .circle3 {
        -webkit-animation-delay: -0.6s;
        animation-delay: -0.6s;
    }

    .container2 .circle3 {
        -webkit-animation-delay: -0.5s;
        animation-delay: -0.5s;
    }

    .container3 .circle3 {
        -webkit-animation-delay: -0.4s;
        animation-delay: -0.4s;
    }

    .container1 .circle4 {
        -webkit-animation-delay: -0.3s;
        animation-delay: -0.3s;
    }

    .container2 .circle4 {
        -webkit-animation-delay: -0.2s;
        animation-delay: -0.2s;
    }

    .container3 .circle4 {
        -webkit-animation-delay: -0.1s;
        animation-delay: -0.1s;
    }

    @-webkit-keyframes bouncedelay {
        0%, 80%, 100% { -webkit-transform: scale(0.0) }
        40% { -webkit-transform: scale(1.0) }
    }

    @keyframes bouncedelay {
        0%, 80%, 100% {
            transform: scale(0.0);
            -webkit-transform: scale(0.0);
        } 40% {
              transform: scale(1.0);
              -webkit-transform: scale(1.0);
          }
    }
</style>
