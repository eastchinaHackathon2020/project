<template>
    <div style="background-color: #F6F6F6">
        <div style="height: 50px">
            <jhi-navbar></jhi-navbar>
        </div>
        <div id="app">
            <router-view></router-view>
        </div>
        <b-modal id="login-page" hide-footer lazy>
            <span slot="modal-title" id="login-title">Sign in</span>
            <login-form></login-form>
        </b-modal>
    </div>
</template>

<script lang="ts" src="./app.component.ts">
</script>

<style scoped>
    #app{
        width: 100%;
        text-align: center;
        padding-left: 70px;
        padding-right: 70px;
        padding-top: 30px;
    }

</style>
