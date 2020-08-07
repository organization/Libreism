<template>
  <div style="display: flex; justify-content: center; height: 100vh; align-items: center;">
    <div
        style="display: flex; align-items: center; border: 10px solid #000; width: 50%; min-width: 300px; height: 70px; font-size: 18px;">
      <input type="text" placeholder="시간을 가져올 서버 주소를 입력해주세요." v-model="servURL" @keydown="keydown($event)"
             style="width: 95%; display: inline-block; border: 0; outline: 0; text-indent: 10px; padding: 0; margin-left: 1px;">
      <span class="mdi mdi-layers-search-outline" style="margin-right: 10px; cursor: pointer;" @click="click"></span>
    </div>
  </div>
</template>

<script>
import router from "@/router";

export default {
  name: "Home",
  data() {
    return {
      servURL: null
    }
  },
  methods: {
    keydown(event) {
      if (event.code === "Enter") this.click();
    },
    click() {
      if (this.servURL === null || !(new RegExp('((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|' + // domain name
          '((\\d{1,3}\\.){3}\\d{1,3}))' + // OR ip (v4) address
          '(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*' + // port and path
          '(\\?[;&a-z\\d%_.~+=-]*)?' + // query string
          '(\\#[-a-z\\d_]*)?$', 'i').test(this.servURL))) {
        return;
      }
      router.push(`${this.servURL}`);
    }
  }
}
</script>