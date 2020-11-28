<template>
    <div id="result" style="background-color: white">
        <div class="jumbotron masthead">
            <div class="container">
                <h1>XAnalysis</h1>
                <h2>您的</h2>
            </div>
        </div>
        <div>
            <b-card no-body class="overflow-hidden" style="max-height: 400px;">
                <b-row no-gutters>

                    <b-col md="5">
                        <b-card-body :title="thisProject.githubAddress">
                            <b-card-text>
                                This is a wider card with supporting text as a natural lead-in to additional content.
                                This content is a little bit longer.
                            </b-card-text>
                        </b-card-body>
                    </b-col>
                    <b-col md="7">
                        <b-card-img src="https://picsum.photos/400/400/?image=20" alt="Image" class="rounded-0"></b-card-img>
                    </b-col>
                </b-row>
            </b-card>
        </div>
    </div>
</template>

<script>


    export default {
        name: "Result",
        data() {
            return{
                thisProject:{},
                warn:'',
                index:0,
            }
        },
        created(){
            this.thisProject=this.$store.getters.thisProject
        },
        mounted() {

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
