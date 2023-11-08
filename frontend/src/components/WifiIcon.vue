<script setup>
import {computed} from "vue";
import {normalizeRSSI, rssiToColor} from "../utils/wifi";

const props = defineProps({
  connected: Boolean,
  rssi: Number
});

const RSSI_MAPPING = {
  3: 'md-wifi',
  2: 'md-wifi2bar',
  1: 'md-wifi1bar'
}

const iconName = computed(() => {
  if (!props.connected) return 'md-wifioff';
  return RSSI_MAPPING[normalizeRSSI(props.rssi)];
});

const iconFill = computed(() => {
  if (!props.connected) return '#000';
  return rssiToColor(props.rssi);
});
</script>

<template>
  <Icon :name="iconName" :fill="iconFill" scale="1.5"/>
</template>

<style scoped>

</style>
