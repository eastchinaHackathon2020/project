<template>
    <div id="wrapper" style="background-color: white">
        <div class="jumbotron masthead">
            <div class="container">
                <h1>XAnalysis</h1>
                <h2>简洁、直观的静态分析工具，让代码更安全、简单。</h2>
                <h5>当前版本： v1.0.0 | 文档更新于：2020-04-25</h5>
                <hr>
                <a style="font-size: 22px;font-weight: normal" @click="jump">点击开始进入静态分析之旅</a>
            </div>
        </div>
    </div>
</template>

<script lang="ts" src="./home.component.ts">
</script>

<style scoped>
        #wrapper{
            box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
            padding: 15px 40px;
        }

        .masthead h1 {
                     font-size: 110px;
                     font-weight:bold;
                     margin-bottom: 30px;
                     color:#ffffff;
                 }

        .btn{
            color: #ffffff;
        }

        .masthead h2 {
            color:#ffffff;
            font-weight:normal;
            margin-bottom:30px;
        }

        .masthead {
            padding:0;
            margin-bottom: 30px;
            margin-top: 30px;
            color: #fff;
        }

        .jumbotron {
            position: relative;
            padding: 180px 180px;
            color: #fff;
            text-align: center;
            text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
            background: #020031;
            background: linear-gradient(45deg,#43d8c9,#035aa6);
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
            box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
        }
</style>
