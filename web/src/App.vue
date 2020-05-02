<template>
	<div>
		<nav class="libreism-info">
			<div class="libreism-info__text" @click="$router.push('/')">Libreism</div>
			<div class="libreism-info__bar"></div>
		</nav>
		<router-view></router-view>
	</div>
</template>

<script>
	export default {
		name: "App",
		data() {
			return {
				standardTime: null,
				servTime: null,
				servURL: null
			}
		}
	};
</script>

<style>
	@import "./assets/main.css";
</style>
