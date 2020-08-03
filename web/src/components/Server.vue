<template>
	<div class="main-content__header">
		<section class="main-content">
			<div v-if="pending">
				시간을 가져오는 중이에요
				<!-- TODO: add progress bar -->
			</div>
			<div v-else-if="servTime === null && standardTime === null">
				시간을 가져올 수 없어요
			</div>
			<div v-else>
				<div class="main-content__title">
					<span class="main-content__url">{{ servURL }}</span>의 시간
				</div>
				<div class="main-content__content">
					<div class="main-content__time">
						<div class="main-content__server_time">{{ format(servTime.getDate()) }}일 {{ format(servTime.getHours()) }}시</div>
						<div class="main-content__standard_time">표준시간대</div>
					</div>
					<div class="main-content__time">
						<div class="main-content__server_time">{{ format(servTime.getMinutes()) }}분</div>
						<div class="main-content__standard_time">{{ format(standardTime.getMinutes()) }}분</div>
					</div>
					<div class="main-content__time">
						<div class="main-content__server_time">{{ format(servTime.getSeconds()) }}초</div>
						<span class="main-content__standard_time" style="position: relative;">
							{{ format(standardTime.getSeconds()) }}초
							<span class="main-content__time_diff">
								{{ standardTime > servTime ? "" : "+" }}
								{{ Math.floor((servTime.getTime() - standardTime.getTime()) / 1000) }}초
							</span>
						</span>
					</div>
				</div>
			</div>
		</section>
	</div>
</template>

<script>
	import axios from "axios";

	export default {
		name: "Server",
		data() {
			return {
				standardTime: null,
				servTime: null,
				servURL: null,
				pending: true
			}
		},
		methods: {
			refreshURL(to) {
				this.servURL = to.path.split("/")[1];
				this.fetchTime();
			},
			async fetchTime() {
				this.servTime = null;
				this.standardTime = null;
				this.pending = true;

				try {
					let res = (await axios.get(`${location.protocol}//${location.hostname}:8080/libreism/api/v1/TimeHttpController/timejson?url=${this.servURL}`)).data;
					let diff = res.network_latency;

					this.servTime = new Date(res.server_time + diff + 1000 * 3600 * 9);
					this.standardTime = new Date(res.standard_time + diff);

					if (isNaN(this.servTime) || isNaN(this.standardTime)) throw new Error();
				} catch (e) {
					this.servTime = null;
					this.standardTime = null;
				} finally {
					this.pending = false;
				}
			},
			format(num) {
				return num >= 10 ? num : `0${num}`;
			},
			addSecond(sec, ms = 0) {
				this.servTime = new Date(this.servTime.setSeconds(this.servTime.getSeconds() + sec, this.servTime.getMilliseconds() + ms));
				this.standardTime = new Date(this.standardTime.setSeconds(this.standardTime.getSeconds() + sec, this.standardTime.getMilliseconds() + ms));
			}
		},
		watch: {
			$route(to) {
				this.refreshURL(to);
			}
		},
		created() {
			this.refreshURL(this.$route);
			let self = this;
			setTimeout(() => setInterval(() => {
				if (!this.pending && this.servTime !== null && this.standardTime !== null) {
					self.addSecond(1, 0);
				}
			}, 1000), 1000 - (Date.now() % 1000));
		}
	};
</script>