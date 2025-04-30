/**
 * Prompt Analyzer
 * 
 * This tool helps analyze the effectiveness of your prompts to AI assistants.
 * It checks for key components and suggests improvements based on best practices.
 */

class PromptAnalyzer {
    constructor() {
        this.promptComponents = [
            { name: 'Context', weight: 0.2, regex: /context|background|situation|I'm working on|about the project/i },
            { name: 'Task', weight: 0.25, regex: /task|goal|objective|need to|would like to|please|can you/i },
            { name: 'Format', weight: 0.15, regex: /format|structure|provide|output|should look like|in the form of/i },
            { name: 'Examples', weight: 0.15, regex: /example|like this|similar to|reference|such as/i },
            { name: 'Constraints', weight: 0.15, regex: /constraint|limit|must|should|requirement|needs to/i },
            { name: 'Clarity', weight: 0.1, regex: /specific|clear|precise|exactly|particular/i }
        ];
        
        this.commonIssues = [
            { issue: 'Vague instructions', regex: /do something|help with this|fix this|make it better|improve/i, suggestion: 'Be more specific about what you want the AI to do' },
            { issue: 'Missing context', regex: /this (code|project|feature)|it doesn't work/i, suggestion: 'Provide more background about the project/problem' },
            { issue: 'Ambiguous requirements', regex: /good|nice|better|improve|enhance|optimize/i, suggestion: 'Define concrete requirements or outcomes' },
            { issue: 'Too broad', regex: /everything|all of|the whole/i, suggestion: 'Break down into smaller, focused requests' },
            { issue: 'Lack of examples', regex: /(how|what) (would|should) (it|this) look/i, suggestion: 'Include examples of desired output' }
        ];
    }
    
    /**
     * Analyzes a prompt and returns a score and suggestions
     * @param {string} prompt - The prompt to analyze
     * @return {Object} Analysis results
     */
    analyzePrompt(prompt) {
        if (!prompt || typeof prompt !== 'string') {
            throw new Error('Invalid prompt provided');
        }
        
        const analysis = {
            score: 0,
            maxScore: 10,
            componentScores: {},
            missingComponents: [],
            detectedIssues: [],
            suggestions: []
        };
        
        // Check for key components
        this.promptComponents.forEach(component => {
            const hasComponent = component.regex.test(prompt);
            const componentScore = hasComponent ? component.weight * 10 : 0;
            
            analysis.componentScores[component.name] = componentScore;
            analysis.score += componentScore;
            
            if (!hasComponent) {
                analysis.missingComponents.push(component.name);
                analysis.suggestions.push(`Add ${component.name.toLowerCase()} information to your prompt`);
            }
        });
        
        // Check for common issues
        this.commonIssues.forEach(issue => {
            if (issue.regex.test(prompt)) {
                analysis.detectedIssues.push(issue.issue);
                analysis.suggestions.push(issue.suggestion);
            }
        });
        
        // Check prompt length
        if (prompt.length < 50) {
            analysis.detectedIssues.push('Prompt too short');
            analysis.suggestions.push('Provide more details to get better results');
        } else if (prompt.length > 500) {
            analysis.detectedIssues.push('Prompt very long');
            analysis.suggestions.push('Consider focusing on the most relevant information');
        }
        
        // Remove duplicate suggestions
        analysis.suggestions = [...new Set(analysis.suggestions)];
        
        return analysis;
    }
    
    /**
     * Provides recommendations to improve a prompt
     * @param {string} prompt - The prompt to improve
     * @return {string} Improved prompt suggestion
     */
    suggestImprovedPrompt(prompt) {
        const analysis = this.analyzePrompt(prompt);
        let improvedPrompt = prompt;
        let recommendations = [];
        
        if (analysis.missingComponents.includes('Context')) {
            recommendations.push(
                'Add context section:\n```\nContext: I\'m working on [project type] using [technologies]. This feature should [purpose].\n```'
            );
        }
        
        if (analysis.missingComponents.includes('Task')) {
            recommendations.push(
                'Add clear task description:\n```\nTask: I need to [specific action] that [specific outcome].\n```'
            );
        }
        
        if (analysis.missingComponents.includes('Format')) {
            recommendations.push(
                'Add format requirements:\n```\nFormat: Please provide [code/explanation/steps] with [specific structure].\n```'
            );
        }
        
        if (analysis.missingComponents.includes('Examples')) {
            recommendations.push(
                'Add examples:\n```\nExample: Something like this: [example]\n```'
            );
        }
        
        if (analysis.missingComponents.includes('Constraints')) {
            recommendations.push(
                'Add constraints:\n```\nConstraints: The solution must [requirement 1], [requirement 2], etc.\n```'
            );
        }
        
        return {
            originalPrompt: prompt,
            score: analysis.score.toFixed(1) + '/' + analysis.maxScore,
            recommendations: recommendations,
            analysis: analysis
        };
    }
    
    /**
     * Tests a prompt by calling the AI and evaluating the response
     * @param {string} prompt - The prompt to test
     * @param {function} aiCallFunction - Function that calls the AI
     * @return {Object} Test results
     */
    async testPrompt(prompt, aiCallFunction) {
        // This is a placeholder - in a real implementation, you would
        // call an actual AI API and evaluate the response
        try {
            const startTime = performance.now();
            const response = await aiCallFunction(prompt);
            const endTime = performance.now();
            
            return {
                prompt: prompt,
                responseTime: endTime - startTime,
                responseLength: response.length,
                response: response
                // Additional metrics would go here
            };
        } catch (error) {
            return {
                error: error.message
            };
        }
    }
    
    /**
     * Compare two different prompts for the same task
     * @param {string} prompt1 - First prompt to compare
     * @param {string} prompt2 - Second prompt to compare
     * @return {Object} Comparison results
     */
    comparePrompts(prompt1, prompt2) {
        const analysis1 = this.analyzePrompt(prompt1);
        const analysis2 = this.analyzePrompt(prompt2);
        
        return {
            prompt1: {
                text: prompt1,
                score: analysis1.score.toFixed(1),
                strengths: Object.entries(analysis1.componentScores)
                    .filter(([_, score]) => score > 0)
                    .map(([component, _]) => component)
            },
            prompt2: {
                text: prompt2,
                score: analysis2.score.toFixed(1),
                strengths: Object.entries(analysis2.componentScores)
                    .filter(([_, score]) => score > 0)
                    .map(([component, _]) => component)
            },
            winner: analysis1.score > analysis2.score ? 'prompt1' : 
                    analysis2.score > analysis1.score ? 'prompt2' : 'tie',
            scoreDifference: Math.abs(analysis1.score - analysis2.score).toFixed(1)
        };
    }
}

// Usage examples

// Example 1: Analyze a single prompt
function analyzeMyPrompt() {
    const analyzer = new PromptAnalyzer();
    const myPrompt = "Can you help me fix this code? It's not working correctly.";
    
    const analysis = analyzer.analyzePrompt(myPrompt);
    console.log("Prompt Score:", analysis.score.toFixed(1) + "/10");
    console.log("Missing Components:", analysis.missingComponents.join(", "));
    console.log("Suggestions:", analysis.suggestions);
}

// Example 2: Get an improved prompt suggestion
function getImprovedPrompt() {
    const analyzer = new PromptAnalyzer();
    const myPrompt = "Help me create a notification system";
    
    const improvement = analyzer.suggestImprovedPrompt(myPrompt);
    console.log("Original Prompt:", improvement.originalPrompt);
    console.log("Score:", improvement.score);
    console.log("Recommendations:");
    improvement.recommendations.forEach((rec, i) => console.log(`${i+1}. ${rec}`));
}

// Example 3: Compare two prompts
function compareMyPrompts() {
    const analyzer = new PromptAnalyzer();
    
    const weakPrompt = "Fix the image display issue please";
    const strongPrompt = `Context: I'm working on a browser game with vanilla JS.
Task: Fix an issue where images are being cut off at the bottom of the screen.
Format: Please provide the CSS changes needed with explanation.
Example: The problem occurs with images like this: [example image]
Constraints: Solution must work on mobile devices too.`;
    
    const comparison = analyzer.comparePrompts(weakPrompt, strongPrompt);
    console.log("Weak Prompt Score:", comparison.prompt1.score);
    console.log("Strong Prompt Score:", comparison.prompt2.score);
    console.log("Winner:", comparison.winner);
    console.log("Score Difference:", comparison.scoreDifference);
}

// To run in browser, uncomment these lines
// analyzeMyPrompt();
// getImprovedPrompt();
// compareMyPrompts();

// Export for Node.js
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { PromptAnalyzer };
}
