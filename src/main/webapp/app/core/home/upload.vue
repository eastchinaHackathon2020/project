<template>
    <div style="height: 100%">
        <div id="wrapper" v-if="!this.loading" style="background-color: white">
            <div class="jumbotron1 masthead1">
                <h1>XAnalysis</h1>
                <h2>简洁、直观的静态分析工具，让代码更安全、简单。</h2>
                <hr>
                <h3 style="font-size: 25px;font-weight: normal;margin-top: 30px">请上传您需要检测的CPP代码文件</h3>
                <div class="uploadDiv1" style="padding-left: 320px;padding-right: 320px;margin-top: 30px">
                    <uploader
                        ref="uploader"
                        :options="uploadOptions1"
                        :autoStart="true"
                        @file-added="onFileAdded1"
                        @file-success="onFileSuccess1"
                        @file-progress="onFileProgress1"
                        @file-error="onFileError1"
                        class="uploader-app"
                        style="background-color: #035aa6"
                    >
                        <uploader-unsupport></uploader-unsupport>
                        <uploader-drop sytle="background: linear-gradient(45deg,#43d8c9,#035aa6);">
                            <uploader-btn style="margin-right:20px;" :attrs="attrs">选择文件</uploader-btn>
                            <uploader-btn :attrs="attrs" directory>选择文件夹</uploader-btn>
                        </uploader-drop>
                        <uploader-list></uploader-list>
                    </uploader>
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
import { Store } from 'vuex';

export default{
    name: "Upload",
    data() {
    return {
      loading:false,
      panelShow: false, //选择文件后，展示上传panel
      collapse: false,
      files: [],
      uploadOptions1: {
        target: "//localhost:8080/upload/single",//上传的接口
        testChunks: false, //是否开启服务器分验
        fileParameterName: "file",//默认的文件参数名
        headers: {},
        query() {},
        categaryMap: { //用于限制上传的类型
          document: ["gif", "jpg", "jpeg", "png", "bmp","cpp"],
        }
      },
      attrs: {
      },
    }
  },

  methods: {
    onFileAdded1(file) {
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
    }
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
