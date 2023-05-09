import { boardCategories, boardDefinitions, labelData } from './data.js';

const renderer = {
	heading(text, level) {
		return `<h3>${text}</h3>`;
	}
};

window.$docsify = {
	name: 'GP2040-CE',
	repo: 'https://github.com/OpenStickCommunity/GP2040-CE',
	homepage: 'https://raw.githubusercontent.com/OpenStickCommunity/GP2040-CE/main/README.md',
	logo: 'assets/images/gp2040-ce-logo.png',
	loadSidebar: '_sidebar.md',
	auto2top: true,
	subMaxLevel: 3,
	themeable: {
		readyTransition : true, // default
		responsiveTables: true  // default
	},
	vueGlobalOptions: {
		directives: {
			VScrollThreshold,
			VueStickyElement,
		},
		data() {
			return {
				labelData,
				selectedLabels: localStorage.getItem('selectedLabels') || 'GP2040',
			};
		}
	},
	vueComponents: {
		'download-box': {
			props: ['release'],
			template: `
				<div class="download-box">
					<a class="download-link" :href="release.link">
						<img class="download-img" :src="'/assets/boards/' + release.key + '.jpg'">
						<div class="download-info">
							<span class="download-title">{{ release.name }}</span>
							<div v-if="release.url">
								<a class="homepage-link" :href="release.url" target="_blank">Website</a>
							</div>
							<div class="download-desc" v-if="release.desc" v-html="release.desc"></div>
						</div>
					</a>
				</div>
			`,
		},

		'download-category': {
			props: ['category'],
			template: `
				<div class="download-category">
					<h3>{{ category }}</h3>
					<p v-if="category === 'Legacy Devices'">The builds for these devices have been deprecated and are no longer supported by GP2040-CE. They may be removed from the releases at some point in the future.</p>
					<div class="download-category-list">
						<download-box
							v-for="release in releases[category]"
							:key="release.title"
							:release="release">
						</download-box>
					</div>
				</div>
			`,
			data() {
				return {
					releases: this.$root.releases,
				};
			},
		},

		'download-page': {
			template: `
				<div v-if="fetched">
					<version-name></version-name>
					<download-category
					v-for="category of boardCategories"
					:key="category"
					:category="category">
					</download-category>
					<div>
						<h3>Flash Nuke</h3>
						<p>
							Use the <a href="https://raw.githubusercontent.com/OpenStickCommunity/GP2040-CE/main/docs/downloads/flash_nuke.uf2">flash_nuke.uf2</a>
							file to completely erase the flash of the RP2040 board. <strong>THIS WILL ERASE ALL SAVED SETTINGS!</strong>
							Use this prior to flashing GP2040-CE on your device if you want to start from a clean slate, or as a troubleshooting step before to reporting an issue.
						</p>
					</div>
					<release-notes></release-notes>
				</div>
			`,
			data() {
				return {
					boardCategories,
					boardDefinitions,
					fetched: false,
					releaseNotes: this.$root.releaseNotes,
					releaseVersion: this.$root.releaseVersion,
					releases: this.$root.releases,
				};
			},
			created() {
				fetch("https://api.github.com/repos/OpenStickCommunity/GP2040-CE/releases/latest", { method: "GET", cache: "force-cache" })
					.then(response => response.json())
					.then(data => {
						const releaseInfo = data;

						this.$root.releaseNotes = releaseInfo.body;
						this.$root.releaseVersion = releaseInfo.name;
						this.$root.releases = releaseInfo.assets
							.filter(a => a.name.startsWith('GP2040-CE_'))
							.sort(a => a.name)
							.map(a => {
								const key = a.name.substring(a.name.lastIndexOf('_') + 1).replace('.uf2', '');
								const board = boardDefinitions[key];
								return {
									...board,
									key,
									img: `/assets/boards/${key}.jpg`,
									link: a.browser_download_url,
								};
							})
							.reduce((p, r) => {
								let values = { ...p };
								if (!values[r.category])
									values[r.category] = [];

								values[r.category].push(r);
								return values;
							}, { });

						this.fetched = true;
					});
			},
		},

		'label-selector': {
			template: `
				<VueStickyElement visibleOnDirection="disabled" class="label-select" stuckClass="sticky-label-select">
					<div class="form-dropdown">
						<select name="labelSelect" v-model="selectedLabels" v-on:change="setSelectedLabels">
							<option v-for="item in labelData" :key="item.name" :value="item.name">{{ item.name }}</option>
						</select>
					</div>
				</VueStickyElement>
			`,
			data() {
				return {
					labelData: this.$root.labelData,
					selectedLabels: this.$root.selectedLabels,
				};
			},
			methods: {
				setSelectedLabels($event) {
					localStorage.setItem('selectedLabels', $event.target.value);
					this.$root.selectedLabels = $event.target.value;
				},
			},
		},

		'hotkey': {
			props: ['buttons'],
			template: `
				<strong>
					<code class="hotkey">{{ buttons.reduce((prev, next) => prev ? prev + " + " + labelData[state.selectedLabels][next] : labelData[state.selectedLabels][next] , "") }}</code>
				</strong>
			`,
			data() {
				return {
					state: this.$root.$data,
					labelData: this.$root.labelData,
				};
			},
		},

		'version-name': {
			template: `<h2>GP2040-CE <strong v-text="version" v-if="version"></strong></h2>`,
			data() {
				return {
					version: this.$root.releaseVersion,
				};
			},
		},

		'release-notes': {
			template: `
				<div v-html="body"></div>
			`,
			data() {
				marked.use({ renderer });
				return {
					body: marked.parse(this.$root.releaseNotes ? "## Release Notes\r\n" + this.$root.releaseNotes : ""),
				};
			},
		},

	}
};
