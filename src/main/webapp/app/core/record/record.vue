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
                <div style="display: flex">
                    <h1 style="font-size: 23px;font-weight: bold;margin-left: 150px;width: 400px;text-align: left">文件名</h1>
                    <h1 style="font-size: 23px;font-weight: bold;width: 400px;text-align: left">报错数</h1>
                    <h1 style="font-size: 23px;font-weight: bold;text-align: left">检测时间</h1>
                </div>
            </b-list-group-item>
            <b-list-group-item v-for="res in this.myResList" :key="res.index">
                <div style="display: flex"  @click="jumpToDetail(res)">
                    <h1 style="font-size: 23px;font-weight: normal;margin-left: 150px;width: 400px;text-align: left">{{res.filename}}</h1>
                    <h1 style="font-size: 23px;font-weight: normal;width: 400px;text-align: left">{{res.warning.split('\n').length-1}}</h1>
                    <h1 style="font-size: 23px;font-weight: normal;text-align: left">{{res.time}}</h1>
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
                myResList:'',
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
            jumpToDetail(res){
                console.log('res')
                this.$store.commit('setPassage',res.content)
                this.$store.commit('setWarning',res.warning)
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
