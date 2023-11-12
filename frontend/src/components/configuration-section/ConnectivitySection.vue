<script setup>

import MQTTSubsection from "./subsection/connectivity/MQTTSubsection.vue";
import WifiSubsection from "./subsection/connectivity/WifiSubsection.vue";
import ConfigurationSection from "../ConfigurationSection.vue";
import Subsection from "../../utils/utils";

const subsections = [
  new Subsection('mqtt', 'MQTT', MQTTSubsection, true),
  new Subsection('wifi', 'WiFi settings', WifiSubsection)
];

</script>

<template>
  <ConfigurationSection :subsections="subsections"/>
</template>

<style scoped>

</style>
