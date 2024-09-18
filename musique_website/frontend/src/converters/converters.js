/**
 * Useful conversions and constants.
 */

/**
 * Gets page width (in millimeters) of A series paper given the size (like for example A4).
 *
 * @param {number} size The size number (like 4 for A4 paper).
 * @returns The page width in millimeters.
 */
export function getASeriesPageWidthMM(size) {
    return ((1 / Math.pow(2, 1 / 4)) * 1000.0) / Math.pow(2, size / 2);
}

/**
 * Gets page height (in millimeters) of A series paper given the size (like for example A4).
 *
 * @param {number} size The size number (like 4 for A4 paper).
 * @returns The page height in millimeters.
 */
export function getASeriesPageHeightMM(size) {
    return getASeriesPageWidthMM(size) * Math.sqrt(2);
}

/**
 * Gets page width (in millimeters) of B series paper given the size (like for example B4).
 *
 * @param {number} size The size number (like 4 for B4 paper).
 * @returns The page width in millimeters.
 */
export function getBSeriesPageWidthMM(size) {
    return getASeriesPageWidthMM(size) * Math.pow(2, 1 / 4);
}

/**
 * Gets page height (in millimeters) of A series paper given the size (like for example B4).
 *
 * @param {number} size The size number (like 4 for B4 paper).
 * @returns The page height in millimeters.
 */
export function getBSeriesPageHeightMM(size) {
    return getASeriesPageHeightMM(size) * Math.pow(2, 1 / 4);
}

var scalingMM = 6.35;
var scalingTenths = 40;

/**
 * Converts millimeters to tenths (i.e. tenths according to musicxml standards, so tenths of a staff space).
 *
 * @param {number} value The value in millimeters.
 * @returns The value converted to tenths.
 */
export function MMToTenths(value) {
    return (scalingTenths / scalingMM) * value;
}

/**
 * Converts tenths (i.e. tenths according to musicxml standards, so tenths of a staff space) to millimeters.
 *
 * @param {number} value The value in tenths.
 * @returns The value converted to millimeters.
 */
export function TenthsToMM(value) {
    return (scalingMM / scalingTenths) * value;
}
