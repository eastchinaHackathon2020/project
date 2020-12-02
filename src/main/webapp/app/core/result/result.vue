<template>
    <div id="result" style="background-color: white">
        <div class="jumbotron masthead">
            <div class="container">
                <h1>Visual Gitwork</h1>
                <h2>您的仓库信息与Gantt图</h2>
            </div>
        </div>
        <div>
            <b-card no-body class="overflow-hidden" style="max-height: 400px;">
                <b-row no-gutters>

                    <b-col md="5">
                        <b-card-body :title="thisProject.githubAddress">
                            <a src="#"   style="color:#43AB5B">
                                <span data-clipboard-action="copy" class="cobyOrderSn" :data-clipboard-text="thisProject.imgUrl" @click="copyLink">{{thisProject.imgUrl}}</span>
                            </a>
                            <b-card-text style="margin-top: 30px">
<!--                            <b-card-text>-->
<!--                                This is a wider card with supporting text as a natural lead-in to additional content.-->
<!--                                This content is a little bit longer.-->
<!--                            </b-card-text>-->
                            <div  v-for="taskElem in this.thisProject.tasks"  style=" margin-left:50px;margin-right: 30px;margin-top: 10px ;display: flex;justify-content:space-around;align-items: center">
                                <b-dropdown
                                    block
                                    split
                                    split-variant="outline-primary"
                                    variant="primary"
                                    :text="taskElem.name"
                                    @click="chooseTask(taskElem)"
                                    size=""
                                    style="width: 350px;margin-right:10px  "
                                >
                                    <div href="#"
                                         v-for="subTaskElem in taskElem.subTask"
                                         style="width: 350px; height: 30px;display: flex;justify-content:space-between;align-items: center"
                                    >
                                        <div style="padding-left: 20px">{{ subTaskElem }}</div>
                                    </div>
                                </b-dropdown>
                            </div>
                            </b-card-text>
                        </b-card-body>
                    </b-col>
                    <b-col md="7">
                        <b-card-img :src="thisProject.imgUrl" alt="Image" class="rounded-0" style=" width: auto;
    height: auto;
    max-width: 90%;
    max-height: 90%;
"></b-card-img>
                    </b-col>
                </b-row>
            </b-card>
        </div>
    </div>
</template>
<!--src="../util/clipboard.min.js"-->
<script >

    // import Clipboard from 'clipboard';
    export default {
        name: "Result",
        data() {
            return{
                thisProject:{},
                warn:'',
                index:0,
                boxTwo: ''
            }
        },
        created(){
            this.thisProject=this.$store.getters.thisProject
        },
        mounted() {
    },
        methods:{
            copyLink() {
                let _this = this;
                let clipboard = new this.clipboard(".cobyOrderSn");
                clipboard.on('success', function () {
                _this.$toast("复制成功")
                });
                clipboard.on('error', function () {
                _this.$toast("复制失败")
                });
                this.boxTwo = ''
                        this.$bvModal.msgBoxOk(' ', {
                        title: '图片链接已复制',
                        size: 'sm',
                        buttonSize: 'sm',
                        okVariant: 'success',
                        headerClass: 'p-2 border-bottom-0',
                        footerClass: 'p-2 border-top-0',
                        centered: true
                        })
                        .then(value => {
                            this.boxTwo = value
                        })
                        .catch(err => {
                            // An error occurred
                        })

            }
            // copy() {
            //     var clipboard = new Clipboard('.tag-read');
            //     clipboard.on('success', e => {
            //         console.log('复制成功')
            //         // 释放内存
            //         clipboard.destroy()
            //     })
            //     clipboard.on('error', e => {
            //         // 不支持复制
            //         console.log('该浏览器不支持自动复制')
            //         // 释放内存
            //         clipboard.destroy()
            //     })
            // }
    }
    }
</script>

<style scoped>
    #result{
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
        padding: 35px 30px 20px 30px;
    }
    .hljs {
        border: 0;
        font-size: 12px;
        display: block;
        padding: 1px;
        margin: 0;
        width: 100%;
        font-weight: 200;
        color: #333;
        white-space: pre-wrap
    }
    .hljs ol {
        list-style: decimal;
        margin: 0px 0px 0 40px !important;
        padding: 0px;
    }
    .hljs ol li {
        list-style: decimal-leading-zero;
        border-left: 1px solid #ddd !important;
        padding: 5px!important;
        margin: 0 !important;
        white-space: pre;
    }


    .masthead h1 {
        font-size: 70px;
        font-weight:bold;
        margin-bottom: 20px;
        color:#ffffff;
    }

    .btn{
        color: #ffffff;
    }
    .masthead h2 {
        color:#ffffff;
        font-weight:normal;
        margin-bottom:15px;
    }

    .masthead1 h2{
        color: #000000;
        font-size: 28px;
        font-weight: normal;
        margin-bottom: 12px;
    }
    .masthead1 p{
        color: #000000;
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
